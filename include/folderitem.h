#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include <QString>
#include <QDir>

struct FolderItem
{
    int fileLevel;

    quint64 fileSize;
    QString absoluteFileName;
    bool isUnique;
    QStringList copiesList;

    QString fileName() const{
        return absoluteFileName.split(QDir::separator()).last();
    }
    QString relativePath() {
        return absoluteFileName.mid(0, absoluteFileName.length() - fileName().length());
    }
};

#endif // FOLDERITEM_H
