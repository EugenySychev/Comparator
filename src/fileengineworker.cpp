#include "fileengineworker.h"
#include "fileengineworker.h"
#include "utils.h"

FileEngineWorker::FileEngineWorker(QObject *parent)
    : QObject(parent)
    , lfList{nullptr}
    , rtList{nullptr}
    , filesList{nullptr}
{

}

void FileEngineWorker::getFileList(QStringList& list, const QString &folderName, const bool recursive)
{
    list.clear();
    list.append(getFileListFromDir(folderName, recursive));
    emit fileListLoaded();
}

QStringList FileEngineWorker::getFileListFromDir(const QString &folderName, const bool recursive) {
    QDir currentDir(folderName);
    QStringList list;
    currentDir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    for (int i = 0; i < currentDir.entryInfoList().size(); i++) {
        list.append(currentDir.entryInfoList().at(i).absoluteFilePath());
    }
    if (recursive) {
        currentDir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        for (int i = 0; i < currentDir.entryList().size(); i++) {
            list.append(getFileListFromDir(folderName + QDir::separator() + currentDir.entryList().at(i), recursive));
        }
    }
    return list;
}

QList<QPair<QString, QStringList> >* FileEngineWorker::getComparedList()
{
    return filesList;
}

void FileEngineWorker::setCompareList(QList<QPair<QString, QStringList>> *list)
{
    filesList = list;
}

void FileEngineWorker::setIncomingList(QStringList* leftList, QStringList* rightList, const quint8 mode)
{
    lfList = leftList;
    rtList = rightList;
    currentMode = mode;
}


void FileEngineWorker::startComparing()
{
    int totalLeft = lfList->size();
    int totalRight = rtList->size();
    canceled = false;
    filesList->clear();
    for (int i = 0; i < totalLeft; i++) {
        if (canceled)
            break;
        emit progressChanged(100 * i / totalLeft);
        QStringList listAt;
        for (int j = 0; j < totalRight; j++) {
            if (canceled)
                break;
            if (compareFiles(lfList->at(i), rtList->at(j)))
            {
                listAt.append(rtList->at(j));
            }
        }
        filesList->append(qMakePair(lfList->at(i), listAt));
    }
    emit progressChanged(100);
    emit compareFinished();
}

void FileEngineWorker::stopComparing()
{
    canceled = true;
}

bool FileEngineWorker::compareFiles(const QString &first, const QString& second)
{
    if ( ( (currentMode & CompareEngine::COMPARE_NAME) != 0) && first.split(QDir::separator()).last() != second.split(QDir::separator()).last())
    {
        return false;
    }

    QFile lhf(first);
    QFile rhf(second);
    if ( ( (currentMode & CompareEngine::COMPARE_SIZE) != 0) && (lhf.size() != rhf.size()))
    {
        return false;
    }

    if ( (currentMode & CompareEngine::COMPARE_CONTENT) != 0) {
        if (lhf.size() != rhf.size())
            return false;
        if (lhf.open(QIODevice::ReadOnly | QIODevice::Text) && rhf.open(QIODevice::ReadOnly| QIODevice::Text))
        {
            quint64 size = lhf.size();
            for (quint64 i = 0; i < size; i++)
            {
                char c1[BLOCK_SIZE], c2[BLOCK_SIZE];
                int compsize1 = lhf.read(c1, BLOCK_SIZE);

                int compsize2 = rhf.read(c2, BLOCK_SIZE);
                if (compsize1 != compsize2) {
                    rhf.close();
                    lhf.close();
                    return false;
                }
                if (strncmp(c1, c2, compsize1) != 0) {
                    rhf.close();
                    lhf.close();
                    return false;
                }
            }
        } else {
            LOG_FUNC() << "Error open " << first << " " << lhf.error();
            rhf.close();
            lhf.close();
            return false;

        }
        rhf.close();
        lhf.close();
    }
    return true;
}
