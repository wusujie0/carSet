import QtQuick 2.4
import QtQuick.Controls 2.0 as Quick2
import QtQuick.Layouts 1.3
import VPlayApps 1.0

import "../pages"

Page {
    NavigationStack {
        id: topNavigation

        Page {

            //useSafeArea: false // 对异形屏幕不冲突
            title: navigation.currentNavigationItem ? navigation.currentNavigationItem.title : ""

            leftBarItem: IconButtonBarItem {
                icon: IconType.alignleft
                onClicked: {
                    topNavigation.popAllExceptFirstAndPush(settingPageComponent)
                }
            }

            rightBarItem: IconButtonBarItem {
                icon: IconType.plus
                onClicked: {
                    topNavigation.popAllExceptFirstAndPush(editPageComponent)
                }
            }

            Component {
                id: settingPageComponent
                SettingPage {
                }
            }

            Component {
                id: editPageComponent
                EditPage {
                }
            }

            Navigation {
                id: navigation
                navigationMode: navigationModeTabs

                NavigationItem {
                    id: messageItem
                    title: "Message"
                    icon: IconType.envelope

                    MessagePage {
                    }
                }

                NavigationItem {
                    id: cardItem
                    title: "Card"
                    icon: IconType.home
                    CardPage {
                    }
                }

                NavigationItem {
                    id: meItem
                    title: "Me"
                    icon: IconType.user
                }
            }
        }
    }
}
