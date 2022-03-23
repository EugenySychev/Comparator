#ifndef COMPAREENGINE_H
#define COMPAREENGINE_H

#include <QObject>
#include "foldermodel.h"
#include <QDebug>

class CompareEngine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant leftModel READ getLeftModel  NOTIFY leftModelChanged)
    Q_PROPERTY(QVariant rightModel READ getRightModel NOTIFY rightModelChanged)
public:
    explicit CompareEngine(QObject *parent = nullptr);
    ~CompareEngine();

    void updateModel();

    QVariant getLeftModel() { return QVariant::fromValue(left_folderModel); }
    QVariant getRightModel() { return QVariant::fromValue(right_folderModel); }

signals:

    void leftModelChanged();
    void rightModelChanged();


private:

    FolderModel* left_folderModel;
    FolderModel* right_folderModel;

};

#endif // COMPAREENGINE_H
