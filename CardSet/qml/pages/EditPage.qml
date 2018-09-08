import QtQuick 2.4
import VPlayApps 1.0
import QtQuick.Controls 1.2

Page {
    id: editPage
    title: "Edit Card"

    signal editPageBack

    Rectangle {
        id: editCardRec
        height: 500
        width: height * 0.618
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Column {
            width: 309
            height: 500
            Rectangle {
                id: editImageRec
                height: editCardRec.height / 2
                width: editCardRec.width
                color: "#eee"

                AppImage {
                    id: editImage
                    autoTransform: true
                    x: 0
                    y: 0
                    width: 309
                    height: 250
                    fillMode: Image.PreserveAspectCrop
                }

                IconButton {
                    id: editImagIcon
                    anchors.centerIn: parent
                    icon: IconType.photo
                    size: 50
                    onClicked: {
                        nativeUtils.displayImagePicker("ImagPicker")
                    }
                }

                Connections {
                    target: nativeUtils
                    onImagePickerFinished: {
                        if (accepted)
                            editImage.source = path
                        editImagIcon.opacity = 0.0
                    }
                }
            }

            Rectangle {
                id: editTitleAndContentRec
                height: editCardRec.height / 2
                width: editCardRec.width
                color: "#ddd"

                AppTextField {
                    id: editTitle
                    x: 8
                    y: 8
                    width: 293
                    height: 44
                    text: "Add the title"
                    font.bold: false
                    font.family: "Verdana"
                    font.pixelSize: sp(20)
                    clearsOnBeginEditing: true
                    backgroundColor: "#ddd"
                }

                AppTextEdit {
                    id: editContent
                    x: 8
                    y: 61
                    width: 293
                    height: 181
                    text: "Add the content"
                    font.pixelSize: sp(15)
                    horizontalAlignment: Text.AlignHCenter

                    wrapMode: TextEdit.WordWrap
                }
            }
        }
    }

    FloatingActionButton {
        id: publishIcon
        icon: IconType.upload
        visible: true
        onClicked: {
            editPageBack()
            account.setTitle(editTitle.text)
            account.setContains(editContent.text)
            account.setPic(editImage.source)
            account.setMessage(editImage.source)
        }
    }
}
