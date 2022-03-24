#ifndef FILEENGINEWORKER_H
#define FILEENGINEWORKER_H

#include <QObject>

#include <QDir>
#include <QThread>
#include <QStringList>
#include "compareengine.h"

#define BLOCK_SIZE 10

class FileEngineWorker : public QObject
{
    Q_OBJECT
public:

    explicit FileEngineWorker(QObject *parent = nullptr);

    QStringList getFileList(const QString& folderName, const bool recursive = false);
    QList<QPair<QString, QStringList> >* getComparedList();
    void setCompareList(const QList<QPair<QString, QStringList> > *list);
    void setIncomingList(const QStringList& leftList, const QStringList& rightList, const CompareEngine::CompareMode mode = CompareEngine::COMPARE_FULL);
    void startComparing();
signals:
    void compareFinished();
    void progressChanged(int value);

private:
    bool compareFiles(const QString& first, const QString &second);
    CompareEngine::CompareMode currentMode;
    QStringList lfList, rtList;
    QList<QPair<QString, QStringList> > *filesList;
};

#endif // FILEENGINEWORKER_H
