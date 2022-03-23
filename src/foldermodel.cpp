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
        return itemList.at(index.row()).absoluteFileName;
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
    LOG_FUNC() << " folder " << folderName;
    QDir currentDir(folderName);
    currentDir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QList<FolderItem> list;
    for (int i = 0; i < currentDir.entryList().size(); i++) {
        FolderItem item;
        item.absoluteFileName = currentDir.entryList().at(i);
        item.fileLevel = level;
        list.append(item);
    }
    currentDir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    for (int i = 0; i < currentDir.entryList().size(); i++) {
        list.append(getCurrentPathList(folderName + QDir::separator() + currentDir.entryList().at(i), level + 1));
    }
    return list;
}

