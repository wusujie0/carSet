import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.1

Page {
    id: registerPage
    width: mainpage.width
    height: mainpage.height
    title: "Register"
    signal registerSucceeded
    property int registerState: 0

    Rectangle {
        id: head
        width: parent.width
        y: mainpage.height * 0.04
        height: mainpage.height * 0.05
        border.color: "blue"
        NavigationBar {
            anchors.bottom: parent.bottom
            IconButton {
                icon: IconType.backward
                size: mainpage.height * 0.03
                onClicked: {
                    registerPage.forceActiveFocus()
                    registerSucceeded()
                }
                anchors.left: parent.left
                color: "white"
                anchors.bottom: parent.bottom
            }
        }
    }

    backgroundColor: Qt.rgba(
                         0, 0, 0,
                         0) // page background is translucent, we can see other items beneath the page

    // login form background
    Rectangle {
        id: loginForm
        anchors.centerIn: parent
        color: "white"
        width: content.width + dp(48)
        height: content.height + dp(16)
        radius: dp(4)
    }

    Timer {
        id: showTime
        interval: 1000
        //        repeat: true
        onTriggered: {
            if (registerPage.registerState == 0) {
                content.opacity = 1
                showMessageRec.visible = false
            } else if (registerPage.registerState == 1) {
                content.opacity = 1
                showMessageRec.visible = false
            } else if (registerPage.registerState == 2) {
                content.opacity = 1
                showMessageRec.visible = false
            } else {
                content.opacity = 1
                txtUsername.text = ""
                txtPassword.text = ""
                confirmPassword.text = ""
                showMessageRec.visible = false
                registerSucceeded()
                showTime.stop()
            }
            //                account.run()
        }
    }

    // login form content
    GridLayout {
        id: content
        anchors.centerIn: loginForm
        columnSpacing: dp(20)
        rowSpacing: dp(10)
        columns: 2

        // headline
        AppText {
            Layout.topMargin: dp(8)
            Layout.bottomMargin: dp(12)
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter
            text: "Register"
        }

        // email text and field
        AppText {
            text: qsTr("UserName")
            font.pixelSize: sp(12)
        }

        AppTextField {
            id: txtUsername
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
        }

        // password text and field
        AppText {
            text: qsTr("Password")
            font.pixelSize: sp(12)
        }

        AppTextField {
            id: txtPassword
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
            echoMode: TextInput.Password
        }

        AppText {
            text: qsTr("ConfirmPW")
            font.pixelSize: sp(12)
        }

        AppTextField {
            id: confirmPassword
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
            echoMode: TextInput.Password
        }

        // column for buttons, we use column here to avoid additional spacing between buttons
        Column {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.topMargin: dp(12)

            // buttons
            AppButton {
                text: qsTr("Sign In")
                flat: false
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    loginPage.forceActiveFocus(
                                ) // move focus away from text fields
                    if (txtUsername.text.length == 0
                            || txtPassword.text.length == 0
                            || confirmPassword.text.length == 0) {
                        showMessage.text = "Message can't be empty"
                        content.opacity = 0.5
                        showMessageRec.visible = true
                        showTime.restart()
                    } else if (txtPassword.text != confirmPassword.text) {
                        showMessage.text = "Entered passwords differ"
                        content.opacity = 0.5
                        showMessageRec.visible = true
                        registerPage.registerState = 1
                        showTime.restart()
                    } else if (txtUsername.text.length > 20) {
                        showMessage.text = "Username is too long"
                        content.opacity = 0.5
                        showMessageRec.visible = true
                        showTime.restart()
                    } else {

                        //                        showMessage.text = "Register success"
                        //                        content.opacity = 0.5
                        //                        showMessageRec.visible = true
                        //                        registerPage.registerState = 2
                        account.setUserName(txtUsername.text)
                        account.setPassword(txtPassword.text)
                        var s = "register " + txtUsername.text + " " + txtPassword.text
                        account.setSendMessage(s)
                        registerPage.forceActiveFocus()
                        showTime.restart()
                    }
                }
            }
        }
    }

    Connections {
        target: account
        onRegisterSuccess: {
            showMessage.text = "Register success"
            content.opacity = 0.5
            showMessageRec.visible = true
            registerPage.registerState = 3
            //            showTime.restart()
        }
        onRegisterFailed: {
            showMessage.text = "The user name already exists"
            content.opacity = 0.5
            showMessageRec.visible = true
            registerPage.registerState = 2
            //            showTime.restart()
        }
    }
    Rectangle {
        id: showMessageRec
        color: "white"
        border.color: "black"
        //        border.width: sp(1)
        height: dp(50)
        width: dp(150)
        anchors.centerIn: parent
        radius: 20
        visible: false
        AppText {
            id: showMessage
            anchors.fill: parent
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            color: "black"
            visible: true
            //            text: qsTr("success login in")
            font.pixelSize: sp(12)
            font.bold: sp(5)
        }
    }
    Component.onCompleted: {
        account.run()
    }
}
