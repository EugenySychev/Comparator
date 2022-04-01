#include "compareengine.h"
#include <string.h>
#include "utils.h"
#include "fileengine.h"


CompareEngine::CompareEngine(QObject *parent)
    : QObject(parent)
    , currentMode{COMPARE_NAME}
    , currentState{IDLE}
    , recursiveScan{true}
{
    left_folderModel = new FolderModel();
    right_folderModel = new FolderModel();
    connect(left_folderModel, &FolderModel::currentPathChanged, this, &CompareEngine::changeLeftModelSource);
    connect(right_folderModel, &FolderModel::currentPathChanged, this, &CompareEngine::changeRightModelSource);

    fileEngine = new FileEngine(this);
    fileEngine->setCompareList(&compareList);
    connect(fileEngine, &FileEngine::compareFinished, this, &CompareEngine::updateCompareResult);
    connect(fileEngine, &FileEngine::progressChanged, this, &CompareEngine::onProgressChanged);
}

CompareEngine::~CompareEngine()
{
    delete left_folderModel;
    delete right_folderModel;
}

void CompareEngine::changeCheckingFormat(CompareMode mode, bool isChecked)
{
    currentMode = isChecked ? currentMode | mode : currentMode & ~mode;
}

void CompareEngine::changeRecursive(bool recursive)
{
    recursiveScan = recursive;
}

void CompareEngine::updateModel()
{
    emit leftModelChanged();
    emit rightModelChanged();
}

void CompareEngine::startComparing()
{
    if (fileEngine){
        if (currentState != COMPARING) {
            currentState = COMPARING;
            emit stateChanged();
            fileEngine->startComparingLists(left_folderModel->getList(), right_folderModel->getList(), currentMode);
        } else {
            currentState = CANCELED;
            emit stateChanged();
            fileEngine->cancelComparing();
        }
    }

}

void CompareEngine::onProgressChanged(int value)
{
    if (currentProgress != value)
    {
        currentProgress = value;
        emit progressChanged();
    }
}

void CompareEngine::changeLeftModelSource(QString sourcePath)
{
    if (fileEngine) {
        QStringList *list = left_folderModel->getList();
        connect(fileEngine, &FileEngine::fileListLoaded, this, &CompareEngine::leftListUpdated);
        fileEngine->getFileList(*list, sourcePath, recursiveScan);
    }
}

void CompareEngine::changeRightModelSource(QString sourcePath)
{
    if (fileEngine) {
        QStringList *list = right_folderModel->getList();
        connect(fileEngine, &FileEngine::fileListLoaded, this, &CompareEngine::rightListUpdated);
        fileEngine->getFileList(*list, sourcePath, recursiveScan);
    }
}

void CompareEngine::rightListUpdated() {
    right_folderModel->listUpdated();
    disconnect(fileEngine, &FileEngine::fileListLoaded, this, &CompareEngine::rightListUpdated);
}

void CompareEngine::leftListUpdated() {
    left_folderModel->listUpdated();
    disconnect(fileEngine, &FileEngine::fileListLoaded, this, &CompareEngine::leftListUpdated);
}

void CompareEngine::updateCompareResult() {
    QStringList leftList;
    QStringList rightList;

    for (int i = 0; i < compareList.size(); i++)
    {
        if (!compareList[i].second.isEmpty()) {
            leftList.append(compareList[i].first);
            for(int j = 0; j < compareList[i].second.size(); j++) {
                if (!rightList.contains(compareList[i].second[j])) {
                    rightList.append(compareList[i].second[j]);
                }
            }
        }
    }
    left_folderModel->changeNonUniue(leftList);
    right_folderModel->changeNonUniue(rightList);
    currentState = IDLE;
    emit stateChanged();
}

void CompareEngine::selectPair()
{
    int i = left_folderModel->getCurrentIndex();
    QStringList rightList;
    if (compareList.size() > 0 && i <= compareList.size())
    {
        if (!compareList[i].second.isEmpty()) {
            for(int j = 0; j < compareList[i].second.size(); j++) {
                if (!rightList.contains(compareList[i].second[j])) {
                    rightList.append(compareList[i].second[j]);
                }
            }
        }
        right_folderModel->changeNonUniue(rightList);
    }
}



