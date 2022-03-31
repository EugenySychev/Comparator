#include "fileengineworker.h"
#include "fileengineworker.h"
#include "utils.h"

FileEngineWorker::FileEngineWorker(QObject *parent) : QObject(parent)
{

}

void FileEngineWorker::getFileList(QStringList& list, const QString &folderName, const bool recursive)
{
    list.clear();
    list.append(getFileListFromDir(folderName, recursive));
    emit fileListLoaded();
}

QStringList FileEngineWorker::getFileListFromDir(const QString &folderName, const bool recursive) {
    LOG_FUNC() << " folder " << folderName;
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
    filesList->clear();
    for (int i = 0; i < totalLeft; i++) {
        emit progressChanged(i / totalLeft);
        LOG_FUNC() << "Compare " << lfList->at(i);

        QStringList listAt;
        for (int j = 0; j < totalRight; j++) {
            LOG_FUNC() << "With " << rtList->at(j);

            if (compareFiles(lfList->at(i), rtList->at(j)))
            {
                LOG_FUNC() << "match found ";
                listAt.append(rtList->at(j));
            }
        }
        filesList->append(qMakePair(lfList->at(i), listAt));
        LOG_FUNC() << "Append " << qMakePair(lfList->at(i), listAt);
    }
    emit compareFinished();
}

bool FileEngineWorker::compareFiles(const QString &first, const QString& second)
{
    QFile lhf("file://"+first);
    QFile rhf("file://"+second);
    if ( ( (currentMode & CompareEngine::COMPARE_NAME) != 0) && first.split(QDir::separator()).last() != second.split(QDir::separator()).last())
    {
        LOG_FUNC() << "Droped by name";
        return false;
    }

    if ( ( (currentMode & CompareEngine::COMPARE_SIZE) != 0) && (lhf.size() != rhf.size()))
    {
        LOG_FUNC() << "Droped by size";
        return false;
    }

    if ( (currentMode & CompareEngine::COMPARE_CONTENT) != 0) {
        quint64 size = lhf.size();
        if (lhf.open(QIODevice::ReadOnly))
        {
            LOG_FUNC() << "Error open " << first << " " << lhf.error();
        }
        if (rhf.open(QIODevice::ReadOnly))
        {
            LOG_FUNC() << "Error open " << second << " " << lhf.error();
        }
        // thinking about optimization
        for (quint64 i = 0; i < size; i++)
        {
            char c1[BLOCK_SIZE], c2[BLOCK_SIZE];
            int compsize1 = lhf.read(c1, BLOCK_SIZE);

            int compsize2 = rhf.read(c2, BLOCK_SIZE);
            if (compsize1 != compsize2) {
                LOG_FUNC() << "Droped by content1";
                rhf.close();
                lhf.close();
                return false;
            }
            if (strncmp(c1, c2, compsize1) != 0) {
                LOG_FUNC() << "Droped by content2";
                rhf.close();
                lhf.close();
                return false;
            }
        }
        rhf.close();
        lhf.close();
    }

    LOG_FUNC() << "Compared " << first << second << first.split(QDir::separator()).last() << second.split(QDir::separator()).last();
    return true;
}
