import QtQuick 2.5
import QtQuick.Controls 2.1
import QtMultimedia 5.5
import MyVideoFilterLib 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("My Video Filter")

    Camera {
        id: camera
    }

    VideoOutput {
        id: videoOutput

        anchors.fill: parent

        autoOrientation: true
        source: camera
        filters: [ videoFilter ]
    }

    MyVideoFilter {
        id: videoFilter

        orientation: videoOutput.orientation
    }

}
