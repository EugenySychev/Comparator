#ifndef COMPAREENGINE_H
#define COMPAREENGINE_H

#include <QObject>
#include <QThread>
#include "foldermodel.h"
#include <QDebug>

class FileEngine;

class CompareEngine : public QObject
{
    Q_OBJECT

    Q_ENUMS(CompareMode);
    Q_ENUMS(CompareState);
    Q_PROPERTY(QVariant leftModel READ getLeftModel  NOTIFY leftModelChanged)
    Q_PROPERTY(QVariant rightModel READ getRightModel NOTIFY rightModelChanged)
    Q_PROPERTY(int progress READ getCurrentProgress NOTIFY progressChanged);
    Q_PROPERTY(int state READ getCurrentState NOTIFY stateChanged);

public:
    enum CompareMode {
        COMPARE_SIZE = 0x1,
        COMPARE_NAME = 0x2,
        COMPARE_CONTENT = 0x04,
        COMPARE_FULL = 0x07
    };

    enum CompareState {
        IDLE = 0,
        COMPARING,
        CANCELED
    };

    Q_INVOKABLE void changeCheckingFormat(CompareMode mode, bool isChecked);
    Q_INVOKABLE void changeRecursive(bool recursive);
    explicit CompareEngine(QObject *parent = nullptr);
    ~CompareEngine();

    void updateModel();
    int getCurrentProgress() { return currentProgress; }
    QVariant getLeftModel() { return QVariant::fromValue(left_folderModel); }
    QVariant getRightModel() { return QVariant::fromValue(right_folderModel); }
    int getCurrentState() { return currentState; }

public slots:
    void startComparing();
    void onProgressChanged(int);
    void changeLeftModelSource(QString);
    void changeRightModelSource(QString);
    void rightListUpdated();
    void leftListUpdated();
    void updateCompareResult();
    void selectPair();

signals:
    void leftModelChanged();
    void rightModelChanged();
    void progressChanged();
    void stateChanged();

private:
    FolderModel* left_folderModel;
    FolderModel* right_folderModel;
    FileEngine *fileEngine;
    int currentProgress;
    quint8 currentMode;
    CompareState currentState;
    bool recursiveScan;

    QList<QPair < QString, QStringList> > compareList;
};

#endif // COMPAREENGINE_H
