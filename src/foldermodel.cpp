#include "foldermodel.h"
#include "utils.h"
#include <QDebug>
FolderModel::FolderModel()
{

}

void FolderModel::changePath(const QString &folder)
{
    bool needScan = false;
    if (m_sourcePath != folder)
        needScan = true;
    m_sourcePath = folder;
    if (m_sourcePath.startsWith("file://"))
        m_sourcePath.remove("file://");
    if (needScan) {
        scanFolder();
        emit currentPathChanged();
    }
}

int FolderModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << itemList.count();
    return itemList.count();
}

QVariant FolderModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "Request " << index << role;
    if (role == FileNameRole) {
        return itemList.at(index.row()).fileName();
    } else if (role == FileSizeRole) {
        return itemList.at(index.row()).fileSize;
    }
    return "";
}


QString FolderModel::getCurrentPath()
{
    return m_sourcePath;
}

void FolderModel::scanFolder()
{
    itemList.clear();
    itemList.append(getCurrentPathList(m_sourcePath, 0));
    beginInsertRows(QModelIndex(), 0,itemList.count());
    endInsertRows();
    emit dataChanged(index(0,0), index(itemList.count(),  1), {  Qt::DisplayRole });
}

QList<FolderItem> FolderModel::getCurrentPathList(const QString& folderName, const int level) {

}

