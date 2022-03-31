#include "fileengine.h"
#include "fileengineworker.h"
#include "utils.h"

FileEngine::FileEngine(QObject *parent)
    : QObject(parent)
    , filesList{nullptr}
{

    workerThread = new QThread();
    fileEngineWorker = new FileEngineWorker();
    fileEngineWorker->moveToThread(workerThread);
    connect( fileEngineWorker, &FileEngineWorker::progressChanged, this, &FileEngine::progressChanged);
    connect( fileEngineWorker, &FileEngineWorker::compareFinished, this, &FileEngine::compareFinished);
}

FileEngine::~FileEngine()
{
    if (workerThread)
    {
        if (workerThread->isRunning())
        {
            workerThread->quit();
        }
        workerThread->deleteLater();
    }
    fileEngineWorker->deleteLater();
}

QStringList FileEngine::getFileList(const QString &folderName, const bool recursive)
{
    if (fileEngineWorker)
        return fileEngineWorker->getFileList(folderName, recursive);
    return QStringList();
}

QList<QPair<QString, QStringList> >* FileEngine::getComparedList()
{
    return filesList;
}

void FileEngine::setCompareList(QList<QPair<QString, QStringList> > *list)
{
    if (list)
        filesList = list;
}

void FileEngine::startComparingLists(const QStringList &leftList, const QStringList &rightList, const quint8 mode)
{
    fileEngineWorker->setIncomingList(leftList, rightList, mode);
    fileEngineWorker->setCompareList(filesList);
    fileEngineWorker->startComparing();
}



