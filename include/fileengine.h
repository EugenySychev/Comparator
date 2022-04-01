#ifndef FILEENGINE_H
#define FILEENGINE_H

#include <QObject>
#include <QDir>
#include <QThread>
#include <QStringList>
#include "compareengine.h"

#define BLOCK_SIZE 10

class FileEngineWorker;

class FileEngine : public QObject
{
    Q_OBJECT
public:


    explicit FileEngine(QObject *parent = nullptr);
    ~FileEngine();

    void getFileList(QStringList& list, const QString& folderName, const bool recursive = false) ;
    QList<QPair<QString, QStringList> >* getComparedList();
    void setCompareList(QList<QPair<QString, QStringList> > *list);
    void startComparingLists(QStringList* leftList, QStringList* rightList, const quint8 mode = CompareEngine::COMPARE_FULL);
    void cancelComparing();
public slots:
    void moveThread();
signals:
    void fileListLoaded();
    void compareFinished();
    void progressChanged(int value);
    void startComparing();
    void stopComparing();

private:
    bool compareFiles(const QString& first, const QString &second, const CompareEngine::CompareMode mode);

    QThread* workerThread;
    QList<QPair<QString, QStringList> > *filesList;

    FileEngineWorker *fileEngineWorker;
};

#endif // FILEENGINE_H
