#include "fileengineworker.h"
#include "fileengineworker.h"
#include "utils.h"

FileEngineWorker::FileEngineWorker(QObject *parent) : QObject(parent)
{

}

QStringList FileEngineWorker::getFileList(const QString &folderName, const bool recursive)
{
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
            list.append(getFileList(folderName + QDir::separator() + currentDir.entryList().at(i)));
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

void FileEngineWorker::setIncomingList(const QStringList &leftList, const QStringList &rightList, const quint8 mode)
{
    lfList = leftList;
    rtList = rightList;
    currentMode = mode;
}


void FileEngineWorker::startComparing()
{
    int totalLeft = lfList.size();
    int totalRight = rtList.size();
    filesList->clear();
    for (int i = 0; i < totalLeft; i++) {
        emit progressChanged(i / totalLeft);
        QStringList listAt;
        for (int j = 0; j < totalRight; j++) {
            if (compareFiles(lfList.at(i), rtList.at(j)))
            {
                listAt.append(rtList.at(i));
            }
        }
        if (listAt.empty())
            filesList->append(qMakePair(lfList.at(i), QStringList()));
        else
            filesList->append(qMakePair(lfList.at(i), listAt));
    }
    emit compareFinished();
}

bool FileEngineWorker::compareFiles(const QString &first, const QString& second)
{
    QFile lhf(first);
    QFile rhf(second);
    if ( ( (currentMode & CompareEngine::COMPARE_NAME) != 0) && first != second)
        return false;

    if ( ( (currentMode & CompareEngine::COMPARE_SIZE) != 0) && (lhf.size() != rhf.size()))
        return false;

    if ( (currentMode & CompareEngine::COMPARE_CONTENT) != 0) {
        quint64 size = lhf.size();

        // thinking about optimization
        for (quint64 i = 0; i < size; i++)
        {
            char c1[BLOCK_SIZE], c2[BLOCK_SIZE];
            int compsize1 = lhf.read(c1, BLOCK_SIZE);
            int compsize2 = rhf.read(c2, BLOCK_SIZE);
            if (compsize1 != compsize2)
                return false;
            if (strncmp(c1, c2, compsize1) != 0)
                return false;
        }
    }

    return true;
}
