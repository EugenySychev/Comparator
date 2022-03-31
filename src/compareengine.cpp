#include "compareengine.h"
#include <string.h>
#include "utils.h"
#include "fileengine.h"


CompareEngine::CompareEngine(QObject *parent)
    : QObject(parent)
    , currentMode{COMPARE_NAME}
{
    left_folderModel = new FolderModel();
    right_folderModel = new FolderModel();
    connect(left_folderModel, &FolderModel::currentPathChanged, this, &CompareEngine::changeLeftModelSource);
    connect(right_folderModel, &FolderModel::currentPathChanged, this, &CompareEngine::changeRightModelSource);

    fileEngine = new FileEngine(this);
    fileEngine->setCompareList(&compareList);
    connect(fileEngine, &FileEngine::compareFinished, this, &CompareEngine::updateCompareResult);
}

CompareEngine::~CompareEngine()
{
    delete left_folderModel;
    delete right_folderModel;
}

void CompareEngine::changeCheckingFormat(CompareMode mode, bool isChecked)
{
    currentMode = isChecked ? currentMode | mode : currentMode & ~mode;
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
    if (fileEngine) {
        fileEngine->startComparingLists(left_folderModel->getList(), right_folderModel->getList(), currentMode);
    }
}

void CompareEngine::onProgressChanged(int value)
{
    currentProgress = value;
    emit progressChanged();
}

void CompareEngine::changeLeftModelSource(QString sourcePath)
{
    if (fileEngine) {
        QStringList *list = left_folderModel->getList();
        connect(fileEngine, &FileEngine::fileListLoaded, this, &CompareEngine::leftListUpdated);
        fileEngine->getFileList(*list, sourcePath, true);
    }
}

void CompareEngine::changeRightModelSource(QString sourcePath)
{
    if (fileEngine) {
        QStringList *list = right_folderModel->getList();
        connect(fileEngine, &FileEngine::fileListLoaded, this, &CompareEngine::rightListUpdated);
        fileEngine->getFileList(*list, sourcePath, true);
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
    LOG_FUNC() << compareList;
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
}



