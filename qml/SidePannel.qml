import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.0

Item {
    id: sidePanelItem
    property variant folderModel: null
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.alignment: Qt.AlignTop
    Layout.minimumWidth: 300
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectFolder: true

        onAccepted: {
            folderModel.changePath(fileDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
        }
    }
    Component {
        id: listDelegate
        Text {
            id: itemTitle
            text: name
            elide: Text.ElideLeft
            width: folderView.width
            color: is_unique ? "black" : "red"

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    console.log(is_unique)

                    folderView.currentIndex = index
                }
            }
        }
    }

    ColumnLayout {
        id: rowLayout
        anchors.fill: parent
        Layout.margins: 10

        RowLayout {
            id: controlLayout

            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            Button {
                id: openButton
                text: qsTr("Open")
                onClicked: {
                    fileDialog.open()
                }
            }

            Text {
                id: currentPathId
                text: folderModel.currentPath
            }
        }

        ListView {
            id: folderView

            Layout.fillWidth: true
            Layout.fillHeight: true
            highlight: Rectangle { color: "lightsteelblue"; }

            delegate: listDelegate
            onCurrentIndexChanged: {
                console.log("Current index changed" + currentIndex)
            }

            model: folderModel

            ScrollBar.vertical: ScrollBar { }
            ScrollBar.horizontal: ScrollBar { }
        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
