import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import Engine 1.0

Window {
    width: 900
    height: 480
    visible: true
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            id: columnLayout
            Layout.fillHeight: true
            Layout.fillWidth: true
            SidePannel {
                id: leftPanel
                folderModel: engine ? engine.leftModel : null
                Layout.margins: 10
            }
            Button {
                id: compareButton
                Layout.alignment: Qt.AlignCenter
                Layout.margins: 10
                implicitWidth: height
                Image {
                    id: icon
                    height: compareButton.height
                    width: compareButton.height
                    source: engine.state === CompareEngine.COMPARING ? "res/cancel.png" : "res/two-arrows-vector.png"
                }
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
                    Layout.alignment: Qt.AlignTop
                    checked: true
                    text: qsTr("Recursive scan directories")
                    onCheckStateChanged: engine.changeRecursive(checkState)
                }
                Text {
                    text: qsTr("Compare by:")
                }

                CheckBox {
                    id: compareName
                    text: qsTr("Name")
                    checked: true
                    onCheckStateChanged: {
                        engine.changeCheckingFormat(CompareEngine.COMPARE_NAME, checkState)
                    }
                }

                CheckBox {
                    id: compareSize
                    text: qsTr("Size")
                    onCheckStateChanged: {
                        engine.changeCheckingFormat(CompareEngine.COMPARE_SIZE, checkState)
                    }
                }

                CheckBox {
                    id: compareContent
                    text: qsTr("Content")
                    onCheckStateChanged: {
                        engine.changeCheckingFormat(CompareEngine.COMPARE_CONTENT, checkState)
                    }
                }
            }
        }
        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            from: 0
            to: 100
            value: engine.progress
        }
    }
}
