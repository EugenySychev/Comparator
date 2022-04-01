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

void FolderModel::setCurrentIndex(const int index)
{
    currentIndex = index;
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
}

int FolderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return itemList.count();
}

QVariant FolderModel::data(const QModelIndex &index, int role) const
{
    if (role == FileNameRole) {
        return itemList.at(index.row()).fileName();
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

int FolderModel::getCurrentIndex()
{
    return currentIndex;
}

