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
    connect( fileEngineWorker, &FileEngineWorker::fileListLoaded, this, &FileEngine::fileListLoaded);
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

void FileEngine::getFileList(QStringList& list, const QString &folderName, const bool recursive)
{
    if (fileEngineWorker) {
        fileEngineWorker->getFileList(list, folderName, recursive);
    }
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

void FileEngine::startComparingLists(QStringList* leftList, QStringList* rightList, const quint8 mode)
{
    fileEngineWorker->setIncomingList(leftList, rightList, mode);
    fileEngineWorker->setCompareList(filesList);
    fileEngineWorker->startComparing();
}



