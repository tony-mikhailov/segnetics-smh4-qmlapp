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
    title: qsTr("Полёт нормальный!")

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
                    color: Globals.topHeaderBackground
                    Text {
                        id: topHeaderText
                        color: Globals.topHeaderText
                        font: Globals.topHeaderFnt
                        width: headerItem.width - dateTimeLayout.width
                        height: 22
                        text: qsTr("Полёт нормальный!")
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
        }
    }

    contentData: [

        SwipeView {
            id: tabBar
            focus: true
            Item {
                id: contentItem
                width: 480
                height: 252
                AnimatedImage {
                    id: backAnimationImg
                    source: "qrc:/img/pakfa.gif"
                }

                Text {
                    x: 20
                    y: 30
                    textFormat: Text.RichText
                    width: 190
                    height: 147
                    wrapMode: "WordWrap"
                    text: qsTr("Это <b><i>rich</i></b>-текст на полупрозрачном фоне и <font color='red'><b>поверх</b> анимации</font>  <i> =)</i>")
//                    font: Qt.font({family: "Roboto", pixelSize: 24, bold: true, color: "#ffffff"})
                    font: Qt.font({family: "Roboto", pixelSize: 24})
                    color: "#ffffff"
                    Rectangle {
                        anchors.fill: parent
                        radius: 5
                        color: "#D0202020"
                        z: parent.z - 1
                    }
                }

            }
        }
    ]

}
