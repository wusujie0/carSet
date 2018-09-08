import QtQuick.Controls 2.3 as QQC2

import VPlayApps 1.0
import QtQuick.Controls 2.0 as QuickControls2
import QtQuick 2.5

Rectangle {
    id: cardThumbnail
    width: dp(330)
    anchors.horizontalCenterOffset: 0
    height: dp(150)
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    anchors.horizontalCenter: parent.horizontalCenter

    AppImage {
        id: cardImage
        x: dp(165)
        width: cardThumbnail.width / 2
        height: cardThumbnail.height
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: parent.verticalCenter
        sourceSize.width: 0
        fillMode: Image.Tile
        source: "1-14092G00422.jpg"
    }

    Text {
        id: title
        width: cardThumbnail.width / 2
        height: dp(40)
        text: qsTr("title")
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        font.wordSpacing: 0
        font.weight: Font.Normal
        style: Text.Normal
        font.bold: true
        font.pixelSize: sp(20)
        font.family: "Verdana"
        fontSizeMode: Text.FixedSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
    }

    Text {
        id: content
        x: 0
        y: dp(40)
        width: cardThumbnail.width / 2
        height: dp(80)
        text: qsTr("content")
        font.letterSpacing: 3
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: sp(15)
    }

    AppImage {
        id: headImage
        x: 0
        y: dp(120)
        width: dp(30)
        height: dp(30)
        source: "1-14092G00422.jpg"
        fillMode: Image.PreserveAspectCrop
    }
    Text {
        id: userName
        x: dp(30)
        y: dp(120)
        width: dp(115)
        height: dp(30)
        text: qsTr("Name")
        renderType: Text.QtRendering
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: sp(18)
    }
}
