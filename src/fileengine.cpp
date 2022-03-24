#include "fileengine.h"
#include "utils.h"

FileEngine::FileEngine(QObject *parent) : QObject(parent)
{
}

FileEngine::~FileEngine()
{

}

QStringList FileEngine::getFileList(const QString &folderName, const bool recursive)
{

}

QList<QPair<QString, QStringList> >* FileEngine::getComparedList()
{
    return filesList;
}

void FileEngine::setCompareList(QList<QPair<QString, QStringList> > *list)
{
    filesList = list;
}

void FileEngine::startComparingLists(const QStringList &leftList, const QStringList &rightList, const CompareEngine::CompareMode mode)
{
    workerThread = new QThread();
    fileEngineWorker = new FileEngineWorker();
    fileEngineWorker->moveToThread(workerThread);
    connect( workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    connect( fileEngineWorker, &FileEngineWorker::progressChanged, this, &FileEngine::progressChanged);
    connect( fileEngineWorker, &FileEngineWorker::compareFinished, this, &FileEngine::compareFinished);
    connect( workerThread, &QThread::started, fileEngineWorker, &FileEngineWorker::startComparing);
    connect( fileEngineWorker, &FileEngineWorker::compareFinished, workerThread, &QThread::quit);
    connect( fileEngineWorker, &FileEngineWorker::compareFinished, fileEngineWorker, &FileEngineWorker::deleteLater);
    fileEngineWorker->setIncomingList(leftList, rightList, mode);
    fileEngineWorker->setCompareList(filesList);
}



