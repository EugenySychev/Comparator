#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include <QString>
#include <QDir>

struct FolderItem
{
    int fileLevel;
    QString absoluteFileName;
    bool isUnique;

    QString fileName() const{
        return absoluteFileName.split(QDir::separator()).last();
    }
};

#endif // FOLDERITEM_H
