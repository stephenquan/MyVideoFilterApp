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
        filters: [ showCornersVideoFilter ]
        // filters: [ nullVideoFilter ]
        // filters: [ greyScaleVideoFilter ]
        // filters: [ blueToRedVideoFilter ]
        // filters: [ editCenterVideoFilter ]
        // filters: [ greyScaleVideoFilter, showCornersVideoFilter ]
    }

    ShowCornersVideoFilter {
        id: showCornersVideoFilter
    }

    NullVideoFilter {
        id: nullVideoFilter
    }

    GreyScaleVideoFilter {
        id: greyScaleVideoFilter
    }

    BlueToRedVideoFilter {
        id: blueToRedVideoFilter
    }

    EditCenterVideoFilter {
        id: editCenterVideoFilter

        angle: 90
        // invert: true
        // mirror: true
    }

}
