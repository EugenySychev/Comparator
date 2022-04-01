#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "folderitem.h"

class FolderModel : public QAbstractListModel
{
    Q_OBJECT

public:
    FolderModel();

    Q_INVOKABLE void changePath(const QString& folder);
    Q_PROPERTY(QString currentPath READ getCurrentPath NOTIFY currentPathChanged)
    Q_INVOKABLE void setCurrentIndex(const int index);
    Q_ENUMS(FilesRoles)

     enum FilesRoles {
         FileNameRole = Qt::UserRole + 1,
         UniqueRole
     };

    QHash<int,QByteArray> roleNames() const override {
            return {
                { FileNameRole, "name" },
                { UniqueRole, "is_unique" }
            };
        }
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void setPath(const QString& sourcePath);

    QString getCurrentPath();

    void changeNonUniue(QStringList list);
    QStringList* getList() { return &simpleList; }
    int getCurrentIndex();
public slots:
    void listUpdated();

signals:
    void currentPathChanged(QString);

private:
    int currentIndex;
    QStringList simpleList;
    QString m_sourcePath;
    QList<FolderItem> itemList;
    QList<FolderItem> getCurrentPathList(const QString &folderName, const int level);

};

#endif // FOLDERMODEL_H
