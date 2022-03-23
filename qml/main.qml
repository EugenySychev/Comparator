import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0

Window {
    width: 640
    height: 480
    visible: true

    RowLayout {
        id: columnLayout
        anchors.fill: parent
        SidePannel {
            id: sidePannel
            folderModel: engine ? engine.leftModel : null
        }

        SidePannel {
            id: sidePannel1
            folderModel: engine ? engine.rightModel : null
        }
    }

}
