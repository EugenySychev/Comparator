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

    Q_PROPERTY(QVariant leftModel READ getLeftModel  NOTIFY leftModelChanged)
    Q_PROPERTY(QVariant rightModel READ getRightModel NOTIFY rightModelChanged)
    Q_PROPERTY(int progress READ getCurrentProgress NOTIFY progressChanged);
public:

    Q_INVOKABLE void changeNameChecking(bool isChecked);
    Q_INVOKABLE void changeSizeChecking(bool isChecked);
    Q_INVOKABLE void changeContentChecking(bool isChecked);

    enum CompareMode {
        COMPARE_SIZE = 0x1,
        COMPARE_NAME = 0x2,
        COMPARE_CONTENT = 0x04,
        COMPARE_FULL = 0x07
    };
    Q_ENUM(CompareMode);

    explicit CompareEngine(QObject *parent = nullptr);
    ~CompareEngine();

    void updateModel();
    int getCurrentProgress() { return currentProgress; }
    QVariant getLeftModel() { return QVariant::fromValue(left_folderModel); }
    QVariant getRightModel() { return QVariant::fromValue(right_folderModel); }

public slots:
    void startComparing();
    void onProgressChanged(int);
    void changeLeftModelSource(QString);
    void changeRightModelSource(QString);

    void rightListUpdated();
    void leftListUpdated();
    void updateCompareResult();
signals:
    void leftModelChanged();
    void rightModelChanged();
    void progressChanged();

private:

    FolderModel* left_folderModel;
    FolderModel* right_folderModel;
    FileEngine *fileEngine;
    int currentProgress;
    quint8 currentMode;

    QList<QPair < QString, QStringList> > compareList;
};

#endif // COMPAREENGINE_H
