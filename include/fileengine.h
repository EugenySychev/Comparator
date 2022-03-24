#ifndef FILEENGINE_H
#define FILEENGINE_H

#include <QObject>
#include <QDir>
#include <QThread>
#include <QStringList>
#include "fileengineworker.h"
#include "compareengine.h"

#define BLOCK_SIZE 10

class FileEngine : public QObject
{
    Q_OBJECT
public:


    explicit FileEngine(QObject *parent = nullptr);
    ~FileEngine();

    QStringList getFileList(const QString& folderName, const bool recursive = false);
    QList<QPair<QString, QStringList> >* getComparedList();
    void setCompareList(QList<QPair<QString, QStringList> > *list);
    void startComparingLists(const QStringList& leftList, const QStringList& rightList, const CompareEngine::CompareMode mode = CompareEngine::COMPARE_FULL);
signals:
    void compareFinished();
    void progressChanged(int value);

private:
    bool compareFiles(const QString& first, const QString &second, const CompareEngine::CompareMode mode);

    QThread* workerThread;
    QList<QPair<QString, QStringList> > *filesList;

    FileEngineWorker *fileEngineWorker;
};

#endif // FILEENGINE_H
