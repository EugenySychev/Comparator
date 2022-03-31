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

void FolderModel::listUpdated()
{
    beginRemoveRows(QModelIndex(), 0, itemList.count());
    itemList.clear();
    endRemoveRows();
    beginInsertRows(QModelIndex(), 0, simpleList.count() - 1);
    for (int i = 0; i < simpleList.size(); i++) {
        FolderItem item;
        item.absoluteFileName = simpleList[i];
        item.isUnique = true;
        itemList.append(item);
    }
    endInsertRows();
//    emit dataChanged(index(0), index(itemList.count()));
}

int FolderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    LOG_FUNC() << itemList.count();
    return itemList.count();
}

QVariant FolderModel::data(const QModelIndex &index, int role) const
{
    LOG_FUNC() << "Request " << index.row() << " a " << role;
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
    LOG_FUNC() << list;
    for (int i = 0; i < itemList.size(); i++) {
        if (list.contains(itemList.at(i).absoluteFileName))
        {
            itemList[i].isUnique = false;
        } else {
            itemList[i].isUnique = true;
        }
    }

    QModelIndex start_index = createIndex(0, 0);
    QModelIndex end_index = createIndex((itemList.count() - 1), 0);
    emit dataChanged(start_index, end_index);
}

