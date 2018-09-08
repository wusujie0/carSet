import VPlayApps 1.0
import QtQuick 2.0

import "../pages"

App {

    // You get free licenseKeys from https://v-play.net/licenseKey
    // With a licenseKey you can:
    //  * Publish your games & apps for the app stores
    //  * Remove the V-Play Splash Screen or set a custom one (available with the Pro Licenses)
    //  * Add plugins to monetize, analyze & improve your apps (available with the Pro Licenses)
    //licenseKey: "<generate one from https://v-play.net/licenseKey>"
    onInitTheme: {
        Theme.platform = "ios"
    }

    property bool userLoggedIn: false

    // login page is always visible if user is not logged in
    LoginPage {
        z: 1 // show login above actual app pages
        visible: opacity > 0
        enabled: visible
        opacity: userLoggedIn ? 0 : 1 // hide if user is logged in
        onLoginSucceeded: userLoggedIn = true

        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        } // page fade in/out
    }

    CardsetMainPage {
        id: mainpage
    }
}
