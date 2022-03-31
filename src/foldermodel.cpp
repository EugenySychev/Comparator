#include "foldermodel.h"
#include "utils.h"
#include <QDebug>
FolderModel::FolderModel()
{

}

void FolderModel::changePath(const QString &folder)
{
    m_sourcePath = folder;
    if (m_sourcePath.startsWith("file://"))
        m_sourcePath.remove("file://");

    emit currentPathChanged(m_sourcePath);

}

void FolderModel::setList(QStringList list)
{
    itemList.clear();
    beginInsertRows(QModelIndex(), 0,list.count());
    for (int i = 0; i < list.size(); i++) {
        FolderItem item;
        item.absoluteFileName = list[i];
        item.isUnique = true;
        itemList.append(item);
    }
    endInsertRows();
    emit dataChanged(index(0,0), index(itemList.count(),  1), {  FileNameRole, UniqueRole });
}

int FolderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return itemList.count();
}

QVariant FolderModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "Request " << index << role;
    if (role == FileNameRole) {
        return itemList.at(index.row()).fileName();
    } else if (role == FileSizeRole) {
        return itemList.at(index.row()).fileSize;
    } else if (role == UniqueRole) {
        return itemList.at(index.row()).isUnique;
    }
    return "";
}


QString FolderModel::getCurrentPath()
{
    return m_sourcePath;
}

void FolderModel::changeNonUniue(QStringList list) {
    for (int i = 0; i < itemList.size(); i++) {
        if (list.contains(itemList.at(i).fileName()))
            itemList[i].isUnique = true;
        else
            itemList[i].isUnique = false;
    }
}

