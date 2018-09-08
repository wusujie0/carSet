import VPlayApps 1.0
import QtQuick 2.0

ListPage {
    id: settingPage
    title: "Setting"
    model: [{
            section: "Personal information",
            text: "My profile"
            //            page: Qt.resolvedUrl("Page.qml")
        }, {
            section: "Personal information",
            text: "My friends"
        }, {
            section: "Personal information",
            text: "My account"
        }, {
            section: "About CardSet",
            text: "User guide"
        }, {
            section: "About CardSet",
            text: "Feedback"
        }, {
            section: "\t",
            text: "Log out"
        }]
    section.property: "section"

    // open configured page when clicked
    onItemSelected: {
        SettingPage.navigationStack.popAllExceptFirstAndPush(model[index].page)
    }
}
