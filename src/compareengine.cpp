#include "compareengine.h"
#include <string.h>
#include "utils.h"


CompareEngine::CompareEngine(QObject *parent) : QObject(parent)
{
    left_folderModel = new FolderModel();
    right_folderModel = new FolderModel();
    fileEngine.setCompareList(&compareList);
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

void CompareEngine::startComparing()
{
    LOG_FUNC();
}

void CompareEngine::onProgressChanged(int value)
{
    currentProgress = value;
    emit progressChanged();
}



