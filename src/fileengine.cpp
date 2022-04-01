#include "fileengine.h"
#include "fileengineworker.h"
#include "utils.h"

FileEngine::FileEngine(QObject *parent)
    : QObject(parent)
    , filesList{nullptr}
{
    workerThread = new QThread(this);
    fileEngineWorker = new FileEngineWorker();
    workerThread->start();
    fileEngineWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, this, &FileEngine::moveThread);
    connect( fileEngineWorker, &FileEngineWorker::progressChanged, this, &FileEngine::progressChanged);
    connect( fileEngineWorker, &FileEngineWorker::compareFinished, this, &FileEngine::compareFinished);
    connect( fileEngineWorker, &FileEngineWorker::fileListLoaded, this, &FileEngine::fileListLoaded);
    connect(this, &FileEngine::startComparing, fileEngineWorker, &FileEngineWorker::startComparing);
    connect(this, &FileEngine::stopComparing, fileEngineWorker, &FileEngineWorker::stopComparing);
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

void FileEngine::moveThread()
{
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
    emit startComparing();
}

void FileEngine::cancelComparing()
{
    emit stopComparing();
}

