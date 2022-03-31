#include "compareengine.h"
#include <string.h>
#include "utils.h"
#include "fileengine.h"


CompareEngine::CompareEngine(QObject *parent)
    : QObject(parent)
    , currentMode{0}
{
    left_folderModel = new FolderModel();
    right_folderModel = new FolderModel();
    connect (left_folderModel, &FolderModel::currentPathChanged, this, &CompareEngine::changeLeftModelSource);
    connect(right_folderModel, &FolderModel::currentPathChanged, this, &CompareEngine::changeRightModelSource);
    fileEngine = new FileEngine(this);
    fileEngine->setCompareList(&compareList);
}

CompareEngine::~CompareEngine()
{
    delete left_folderModel;
    delete right_folderModel;
}

void CompareEngine::changeNameChecking(bool isChecked)
{
    currentMode = isChecked ? currentMode | COMPARE_NAME : currentMode & ~COMPARE_NAME;
    LOG_FUNC() << currentMode;
}

void CompareEngine::changeSizeChecking(bool isChecked)
{
    currentMode = isChecked ? currentMode | COMPARE_SIZE : currentMode & ~COMPARE_SIZE;
    LOG_FUNC() << currentMode;
}

void CompareEngine::changeContentChecking(bool isChecked)
{
    currentMode = isChecked ? currentMode | COMPARE_CONTENT : currentMode & ~COMPARE_CONTENT;
    LOG_FUNC() << currentMode;
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

void CompareEngine::changeLeftModelSource(QString sourcePath)
{
    if (fileEngine)
        left_folderModel->setList(fileEngine->getFileList(sourcePath, true));
}

void CompareEngine::changeRightModelSource(QString sourcePath)
{
    if (fileEngine)
        right_folderModel->setList(fileEngine->getFileList(sourcePath, true));
}



