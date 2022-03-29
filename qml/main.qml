import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

Window {
    width: 640
    height: 480
    visible: true

    RowLayout {
        id: columnLayout
        anchors.fill: parent
        SidePannel {
            id: leftPanel
            folderModel: engine ? engine.leftModel : null
            Layout.margins: 10
        }
        Button {
            id: compareButton
            text: qsTr("Compare")
            Layout.alignment: Qt.AlignTop
            Layout.margins: 10
            onClicked: {
                engine.startComparing()
            }
        }        
        SidePannel {
            id: rightPanel
            folderModel: engine ? engine.rightModel : null
            Layout.margins: 10
        }
        ColumnLayout {
            id: compareTypeLayout

            CheckBox {
                id: compareName
                text: qsTr("Name")
                onCheckStateChanged: {
                    engine.changeNameChecking(checkState)
                }
            }

            CheckBox {
                id: compareSize
                text: qsTr("Size")
                onCheckStateChanged: {
                    engine.changeSizeChecking(checkState)
                }
            }

            CheckBox {
                id: compareContent
                text: qsTr("Content")
                onCheckStateChanged: {
                    engine.changeContentChecking(checkState)
                }
            }
        }
    }

}
