//卡片详情页
import QtQuick.Controls 2.3 as QQC2
import VPlayApps 1.0
import QtQuick.Controls 2.0 as QuickControls2
import QtQuick 2.5


//样本
Rectangle {
    id: rectangle
    width: height * 0.618
    height: 500
    border.color: "#e7e9ea"
    border.width: 0
    anchors.centerIn: parent

    AppImage {
        id: appImage
        x: 0
        y: 0
        width: 309
        height: 250
        anchors.horizontalCenter: parent.horizontalCenter
        sourceSize.width: 0
        fillMode: Image.PreserveAspectCrop
        source: "1-14092G00422.jpg"
    }

    Text {
        id: text1
        x: 8
        y: 256
        width: 293
        height: 40
        text: qsTr("Title")
        anchors.horizontalCenter: parent.horizontalCenter
        font.wordSpacing: 0
        font.bold: true
        font.pixelSize: 20
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: text2
        x: 8
        y: 305
        width: 293
        height: 144
        text: qsTr("Content")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 15
    }

    Text {
        id: text3
        x: 215
        y: 458
        width: 86
        height: 34
        text: qsTr("2018.7.11")
        verticalAlignment: Text.AlignBottom
        horizontalAlignment: Text.AlignRight
        font.pixelSize: 12
    }

    Text {
        id: text4
        x: 48
        y: 458
        width: 161
        height: 34
        text: qsTr("Name")
        verticalAlignment: Text.AlignBottom
        font.pixelSize: 15
    }

    AppImage {
        id: appImage1
        x: 8
        y: 458
        width: 34
        height: 34
        fillMode: Image.PreserveAspectFit
        source: "1-14092G00422.jpg"
    }

    Rectangle {
        id: rectangle1
        x: 55
        y: 298
        width: 200
        height: 1
        color: "#26292b"
    }
}
