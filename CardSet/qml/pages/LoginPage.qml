import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.1

Page {
    id: loginPage
    width: mainpage.width
    height: mainpage.height
    title: "Login"
    signal loginSucceeded
    property int loginState: 0

    backgroundColor: Qt.rgba(
                         0, 0, 0,
                         0.75) // page background is translucent, we can see other items beneath the page

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
            if (loginPage.loginState == 0) {
                content.opacity = 1
                showMessageRec.visible = false
                loginPage.forceActiveFocus()
            } else {
                loginSucceeded()
                showMessageRec.visible = false
                txtUsername.text = ""
                txtPassword.text = ""
                content.opacity = 1
            }
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
            text: "Login"
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

        // column for buttons, we use column here to avoid additional spacing between buttons
        Column {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.topMargin: dp(12)

            // buttons
            AppButton {
                text: qsTr("Login")
                flat: false
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    // move focus away from text fields

                    // simulate successful login
                    if (txtUsername.text.length == 0
                            || txtPassword.text.length == 0) {
                        showMessage.text = "Message can't be empty"
                        content.opacity = 0.5
                        showMessageRec.visible = 1
                        showTime.restart()
                    } else {
                        var send = "login " + txtUsername.text + " " + txtPassword.text
                        account.setSendMessage(send)
                        loginPage.forceActiveFocus()
                        //                        showMessage.text = "login Success"
                        //                        content.opacity = 0.5
                        //                        showMessageRec.visible = 1
                        //                        account.setUserName(txtUsername.text)
                        //                        account.setPassword(txtPassword.text)
                        //                        loginPage.loginState = 1
                        showTime.restart()
                    }
                }
            }

            AppButton {
                text: qsTr("No account yet? Register now")
                flat: true
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    loginPage.forceActiveFocus(
                                ) // move focus away from text fields

                    // call your server code to register here
                    // console.debug("registering...")
                    register.visible = true
                }
            }
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

    RegisterPage {
        width: dp(20)
        height: dp(10)
        id: register
        visible: false
        onRegisterSucceeded: register.visible = false
        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }
    }
    Connections {
        target: account
        onLoginSuccess: {
            showMessage.text = "Login success"
            content.opacity = 0.5
            showMessageRec.visible = true
            loginPage.loginState = 1
            account.setUserName(txtUsername.text)
            account.setPassword(txtPassword.text)
            txtPassword.focus = false
            //            showTime.restart()
        }
        onLoginFailed: {
            showMessage.text = "Login failed,Please check your input"
            content.opacity = 0.5
            showMessageRec.visible = true
            //            showTime.restart()
        }
    }
}
