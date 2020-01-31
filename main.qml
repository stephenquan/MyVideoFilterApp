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

            MouseArea {
                anchors.fill: parent

                onClicked: nextCamera()
            }
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

        Button {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 10
            visible: videoFilterName === "captureVideoFilter"
            text: qsTr( "Capture" )
            font.pointSize: 12
            onClicked: captureVideoFilter.capture();
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.5
        visible: capturedImage.visible
    }

    Image {
        id: capturedImage
        anchors.centerIn: parent
        width: parent.width * 9 / 10
        height: parent.height * 9 / 10
        visible: false
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            onClicked: {
                capturedImage.visible = false;
            }
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
                "captureVideoFilter"
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
                    case "captureVideoFilter": videoOutput.filters = [ captureVideoFilter ]; break;
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

        videoOutputOrientation: videoOutput.orientation
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

    CaptureVideoFilter {
        id: captureVideoFilter

        videoOutputOrientation: videoOutput.orientation

        onCaptured: {
            capturedImage.source = imageUrl;
            capturedImage.visible = true;
        }
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
