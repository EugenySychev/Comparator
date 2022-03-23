#include "compareengine.h"


CompareEngine::CompareEngine(QObject *parent) : QObject(parent)
{
    left_folderModel = new FolderModel();
    right_folderModel = new FolderModel();
}

CompareEngine::~CompareEngine()
{
    delete left_folderModel;
    delete right_folderModel;
}

void CompareEngine::updateModel()
{
    emit leftModelChanged();
    emit rightModelChanged();
}


