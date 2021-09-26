import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

import globals.gui.qmlapp 1.0;

ApplicationWindow {
    id: root
    visible: true
    width: 480
    height: 272
    title: qsTr("App Title") 


    Component.onCompleted: {

    }

    header: Item {
        width: 480
        height: 20

        id: headerItem
        Column {
            id: headerColumn
            Row {
                Rectangle {
                    id: topHeaderRect
                    width: 480
                    height: 20

                    Text {
                        id: topHeaderText
                        color: Globals.topHeaderText
                        font: Globals.topHeaderFnt
                        width: headerItem.width - dateTimeLayout.width //headerItem.width - topHeaderTimeText.width
                        height: 22
                        text: "Qml Application"
                        verticalAlignment: Text.AlignBottom
                    }
                    Row {
                        id: dateTimeLayout
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom


                        Text {
                            id: topHeaderDateText
                            color: Globals.topHeaderText
                            font: Globals.dateHeaderFnt
                            horizontalAlignment: Text.AlignRight
                            leftPadding: 1
                            topPadding: 3
                        }

                        Text {
                            id: topHeaderTimeText
                            color: Globals.topHeaderText
                            font: Globals.timeHeaderFnt
                            horizontalAlignment: Text.AlignRight
                            leftPadding: 3
                            rightPadding: 1
                            verticalAlignment: Text.AlignBottom
                        }

                    }
                    layer.enabled: true
                    layer.effect: DropShadow {
                        transparentBorder: true
                        verticalOffset: 2
                        radius: 8.0
                        samples: 18

                    }
                }
            }
        }
    }

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: {
            var d = new Date();
            topHeaderDateText.text = Qt.formatDateTime(d, ("yyyy.MM.dd"));
            topHeaderTimeText.text = Qt.formatDateTime(d, ("HH:mm:ss"));
            //topHeaderText.width = dateTimeLayout.width;
        }
    }

    contentData: [

        Text {
          text: "text"
        }
    ]

    footer: Item {
        id: footerItem
        width: 480
        height: 40
        Text {
           text: "text"
        }
    }
}
