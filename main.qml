import QtQuick 2.5
import QtQuick.Controls 2.1
import QtMultimedia 5.5
import MyVideoFilterLib 1.0
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

ApplicationWindow {
    id: app

    visible: true
    width: 640
    height: 480
    title: qsTr("My Video Filter")

    property string videoFilterName: "showCornersVideoFilter"
    property var cameras: QtMultimedia.availableCameras
    property var cameraIndex: -1
    property string cameraDisplayName: ""
    property var cameraInfo: null

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    VideoOutput {
        id: videoOutput

        anchors.fill: parent
        autoOrientation: true
        source: camera
        filters: [ showCornersVideoFilter ]

        Rectangle {
            anchors.fill: cameraDisplayNameText
            anchors.margins: -cameraDisplayNameText.height / 2
            radius: height / 2
            color: "#808080"
            opacity: 0.5
        }

        Text {
            id: cameraDisplayNameText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: 10
            color: "yellow"
            font.pointSize: 12
            text: getCameraDisplayName( camera.deviceId )
            visible: !videoFilterMenu.visible
        }

        Rectangle {
            anchors.fill: videoFilterText
            anchors.margins: -videoFilterText.height / 2
            radius: height / 2
            color: "#808080"
            opacity: 0.5
            visible: !videoFilterMenu.visible
        }

        Text {
            id: videoFilterText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.margins: 10
            color: "yellow"
            font.pointSize: 12
            text: videoFilterName
            visible: !videoFilterMenu.visible

            MouseArea {
                anchors.fill: parent

                onClicked: videoFilterMenu.open()
            }
        }

        Rectangle {
            anchors.fill: cameraIcon
            anchors.margins: -16
            radius: height / 2

            color: "#808080"
            opacity: 0.5
        }

        Image {
            id: cameraIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 32

            readonly property string svg: '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 32 32"><path d="M21 10.5a7.5 7.5 0 1 0 7.5 7.5 7.5 7.5 0 0 0-7.5-7.5zm0 14.1a6.6 6.6 0 1 1 6.6-6.6 6.607 6.607 0 0 1-6.6 6.6zM10 12.5v.5H3v-1h7v.5zM29.5 28h-27A2.503 2.503 0 0 1 0 25.5v-14A2.503 2.503 0 0 1 2.5 9H4V8h6v1h4a1.001 1.001 0 0 0 1-1 2.002 2.002 0 0 1 2-2h8a2.002 2.002 0 0 1 2 2 1.001 1.001 0 0 0 1 1h1.5a2.503 2.503 0 0 1 2.5 2.5v14a2.503 2.503 0 0 1-2.5 2.5zm-27-18A1.502 1.502 0 0 0 1 11.5v14A1.502 1.502 0 0 0 2.5 27h27a1.502 1.502 0 0 0 1.5-1.5v-14a1.502 1.502 0 0 0-1.5-1.5H28a2.002 2.002 0 0 1-2-2 1.001 1.001 0 0 0-1-1h-8a1.001 1.001 0 0 0-1 1 2.002 2.002 0 0 1-2 2H9V9H5v1zm18.5.5a7.5 7.5 0 1 0 7.5 7.5 7.5 7.5 0 0 0-7.5-7.5zm0 14.1a6.6 6.6 0 1 1 6.6-6.6 6.607 6.607 0 0 1-6.6 6.6zM10 12.5v.5H3v-1h7v.5zM29.5 28h-27A2.503 2.503 0 0 1 0 25.5v-14A2.503 2.503 0 0 1 2.5 9H4V8h6v1h4a1.001 1.001 0 0 0 1-1 2.002 2.002 0 0 1 2-2h8a2.002 2.002 0 0 1 2 2 1.001 1.001 0 0 0 1 1h1.5a2.503 2.503 0 0 1 2.5 2.5v14a2.503 2.503 0 0 1-2.5 2.5zm-27-18A1.502 1.502 0 0 0 1 11.5v14A1.502 1.502 0 0 0 2.5 27h27a1.502 1.502 0 0 0 1.5-1.5v-14a1.502 1.502 0 0 0-1.5-1.5H28a2.002 2.002 0 0 1-2-2 1.001 1.001 0 0 0-1-1h-8a1.001 1.001 0 0 0-1 1 2.002 2.002 0 0 1-2 2H9V9H5v1z"/></svg>'

            width: 32
            height: 32
            source: "data:image/svg+xml;utf8,%1".arg( svg )
            sourceSize: Qt.size( width, height )

            MouseArea {
                anchors.fill: parent
                onClicked: nextCamera()
            }
        }

        ColorOverlay {
            anchors.fill: cameraIcon
            source: cameraIcon
            color: "yellow"
        }

    }

    Menu {
        id: videoFilterMenu

        anchors.centerIn: parent
        width: parent.width / 2

        Repeater {
            model: [
                "showCornersVideoFilter",
                "nullVideoFilter",
                "greyScaleVideoFilter",
                "blueToRedVideoFilter",
                "editCenterVideoFilter"
            ]

            MenuItem {
                text: modelData
                font.pointSize: 12
                highlighted:  modelData === videoFilterName

                onTriggered: {
                    videoFilterName = modelData;
                    switch ( videoFilterName )
                    {
                    case "nullVideoFilter": videoOutput.filters = [ nullVideoFilter ]; break;
                    case "showCornersVideoFilter": videoOutput.filters = [ showCornersVideoFilter ]; break;
                    case "greyScaleVideoFilter": videoOutput.filters = [ greyScaleVideoFilter ]; break;
                    case "blueToRedVideoFilter": videoOutput.filters = [ blueToRedVideoFilter ]; break;
                    case "editCenterVideoFilter": videoOutput.filters = [ editCenterVideoFilter ]; break;
                    }
                }
            }
        }
    }

    Camera {
        id: camera
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

        //angle: angleSlider.value
        angle: 90
        invert: false // invertCheckBox.checked
        //mirror: mirrorCheckBox.checked
    }

    Timer {
        id: timer
        running: true
        onTriggered: editCenterVideoFilter.angle = ( editCenterVideoFilter.angle + 90  ) % 90;
    }

    function getCameraDisplayName( cameraId ) {
        const cameras = QtMultimedia.availableCameras;
        if ( !cameras || !cameras.length ) return;
        for ( const cameraInfo of cameras ) {
            if ( cameraInfo.deviceId === camera.deviceId ) {
                return cameraInfo.displayName;
            }
        }
        return null;
    }

    function findCameraIndex( cameraId ) {
        for ( let cameraIndex = 0; cameraIndex < cameras.length; cameraIndex++ ) {
            if ( cameras[ cameraIndex ].deviceId === cameraId ) {
                return cameraIndex;
            }
        }
        return -1;
    }

    function nextCamera() {
        cameras = QtMultimedia.availableCameras;
        if ( !cameras || !cameras.length ) {
            cameraIndex = -1;
            cameraInfo = null;
            cameraDisplayName = "";
            return;
        }
        cameraIndex = ( cameraIndex + 1 ) % cameras.length;
        cameraInfo = cameras[ cameraIndex ];
        cameraDisplayName = cameraInfo.displayName;
        camera.stop();
        camera.deviceId = cameras[ cameraIndex ].deviceId;
        camera.start();
    }

    Component.onCompleted: {
        cameras = QtMultimedia.availableCameras;
        cameraIndex = findCameraIndex( camera.deviceId );
        if ( cameraIndex === -1 ) return;
        cameraInfo = cameras[ cameraIndex ];
        cameraDisplayName = cameraInfo.displayName;
    }

}
