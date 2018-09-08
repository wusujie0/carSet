import QtQuick 2.4
import VPlayApps 1.0

ListPage {
    id: messagePage

    property var friendstatus: []
    property var friends: []
    property var friendsnum: []
    property var friendsnumstr: []

    emptyText.text: qsTr("No messages")

    // Use a predefined delegate but change some of its layout parameters
    delegate: SimpleRow {
        image.radius: image.height
        image.fillMode: Image.PreserveAspectCrop
        autoSizeImage: true
        style.showDisclosure: false
        imageMaxSize: dp(48)
        detailTextItem.maximumLineCount: 1
        detailTextItem.elide: Text.ElideRight

        onSelected: {
            topNavigation.popAllExceptFirstAndPush(detailPageComponent, {
                                                       person: item.text
                                                   })
            var s = "receiveChatLog " + account.userName + " " + item.text + " on"
            account.setSendMessage(s)
            var ss = "getfriendsMes " + account.userName
            account.setSendMessage(ss)
        }
    }

    Component {
        id: detailPageComponent
        ConversationPage {
        }
    }

    Connections {
        target: account
        onShowFriends: {
            friends = vec
            friendsnum = vec_num
            friendsnumstr = friendsnum.toString()
            console.log(friends)
            console.log(friendsnum)
            console.log(friendsnumstr)
            friendstatus = []
            for (var i = 0; i < friends.length; i++) {
                friendstatus = friendstatus.concat({
                                                       text: friends[i],
                                                       detailText: friendsnum[i].toString(
                                                           ),
                                                       image: Qt.resolvedUrl(
                                                                  "../portrait0.jpg")
                                                   })
            }
        }
    }

    // 模型应该从消息后端加载
    model: {
        return friendstatus
    }
    //    model: [{
    //            text: "Keen",
    //            detailText: "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh",
    //            image: Qt.resolvedUrl("../portrait0.jpg")
    //        }, {
    //            text: "zhangrui",
    //            detailText: "2333333336666",
    //            image: Qt.resolvedUrl("../portrait1.jpg")
    //        }]
}
