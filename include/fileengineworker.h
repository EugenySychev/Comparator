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

    void getFileList(QStringList &list, const QString& folderName, const bool recursive = false);
    QList<QPair<QString, QStringList> >* getComparedList();
    void setCompareList(QList<QPair<QString, QStringList> > *list);
    void setIncomingList(QStringList *leftList, QStringList *rightList, const quint8 mode = CompareEngine::COMPARE_FULL);
    void startComparing();
signals:
    void fileListLoaded();
    void compareFinished();
    void progressChanged(int value);

private:
    QStringList getFileListFromDir(const QString &folderName, const bool recursive);
    bool compareFiles(const QString& first, const QString &second);
    quint8 currentMode;
    QStringList* lfList;
    QStringList* rtList;
    QList<QPair<QString, QStringList> > *filesList;
};

#endif // FILEENGINEWORKER_H
