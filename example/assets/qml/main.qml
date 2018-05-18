import QtQuick 2.9
import QtQuick.Controls 2.2


import SGUtil 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumWidth: 900
    minimumHeight: 600

    //hobot-defined control which play video from ipc
    SGView {
        id: renderer
        anchors.fill: parent

        //set background color
        background: "green"
    }

    //QML standard Text
    Text {
        id: label
        //anchors.centerIn: parent
        text: "Simple Demo\n简单应用"
        color: "yellow"
        font.pointSize: 24

        //just for demo more interesting
        PathAnimation {
            path: Path {
                startX: 0; startY: 0
                PathCurve {x: window.width*0.2; y: window.height*0.7}
                PathCurve {x: window.width-label.width; y: window.height-label.height}
                PathCurve {x: window.width*0.8; y: window.height*0.3}
                PathCurve {x: startX; y: startY}
            }

            target: label
            duration: 4000
            easing.type: Easing.OutInQuad
            loops: Animation.Infinite
            running: true
        }
    }
}
