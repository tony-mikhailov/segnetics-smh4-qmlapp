import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3

import dev.pump.station.gui 1.0

Item {

    property int valueHeight: 19
    id: root

    //Keys.forwardTo:  [varsListView]
    focus: true

    Keys.onReturnPressed: {
        if (swipeView.currentIndex === Globals.mainViewIndex) {
            device.startMining();
            event.accepted = false;
        }
        event.accepted = false;
    }

    Rectangle {
        anchors.fill: parent


        Row {
            Column {
                width: 480
                Text {
                    width: 480
                    text: qsTr("Параметры работы") + tswitch.lang
                    font: Globals.l2HeaderTitleFnt
                    Rectangle {
                        anchors.fill: parent
                        z: -1
                        color: Globals.l2HeaderTitleBackground
                    }

                    layer.enabled: true
                    layer.effect: DropShadow {
                        verticalOffset: 2
                        radius: 8.0
                        samples: 17
                        color: Globals.shadow
                    }

                }
                ValueLabel {
                    varNameText: qsTr("Дебит") + tswitch.lang
                    valueText: "" + 20.3
                    unitsText: qsTr("м3") + tswitch.lang
                    width: parent.width
                    height: valueHeight
                    backgroundVisible: false
                }
                ValueLabel {
                    varNameText: qsTr("Режим") + tswitch.lang
                    valueText: device.speedmodeToString(device.speedMode)
                    unitsText: ""
                    width: parent.width
                    height: valueHeight
                }
                ValueLabel {
                    varNameText: qsTr("Максимальная скорость") + tswitch.lang
                    valueText: "" + device.P_Nomax
                    unitsText: qsTr("х/мин") + tswitch.lang
                    width: parent.width
                    height: valueHeight
                    visible: device.speedMode == 0
               //     valueWidth: 60
                    backgroundVisible: false
                }
                ValueLabel {
                    varNameText: qsTr("Минимальная скорость") + tswitch.lang
                    valueText: "" + device.P_Nomin
                    unitsText: qsTr("х/мин") + tswitch.lang
                    width: parent.width
                    height: valueHeight
                    visible: device.speedMode == 0
                 //   valueWidth: 65
                }
                ValueLabel {
                    varNameText: qsTr("Cкорость вверх") + tswitch.lang
                    valueText: "" + device.P_N2u
                    unitsText: qsTr("х/мин") + tswitch.lang
                    width: parent.width
                    height: valueHeight
                    visible: device.speedMode == 2
                    backgroundVisible: false
                }
                ValueLabel {
                    varNameText: qsTr("Скорость вниз") + tswitch.lang
                    valueText: "" + device.P_N2d
                    unitsText: qsTr("х/мин") + tswitch.lang
                    width: parent.width
                    height: valueHeight
                    visible: device.speedMode == 2
                }
                ValueLabel {
                    varNameText: qsTr("Скорость вверх/вниз") + tswitch.lang
                    valueText: "" + device.P_N1
                    unitsText: qsTr("х/мин") + tswitch.lang
                    width: parent.width
                    height: valueHeight
                    visible: device.speedMode == 1
                    backgroundVisible: false

                }
                ValueLabel {
                    varNameText: qsTr("Наполнение насоса") + tswitch.lang
                    valueText: "98"
                    unitsText: qsTr("%") + tswitch.lang
                    width: parent.width
                    height: valueHeight
                    backgroundVisible: device.speedMode == 1

                }

                ValueLabel {
                    varNameText: qsTr("Максимальная нагрузка") + tswitch.lang
                    valueText: "99999"
                    unitsText: qsTr("кг")+ tswitch.lang
                    width: parent.width
                    height: valueHeight
                    backgroundVisible: (device.speedMode != 1)

                }
                ValueLabel {
                    varNameText: qsTr("Минимальная нагрузка")+ tswitch.lang
                    valueText: "99999"
                    unitsText: qsTr("х/мин")+ tswitch.lang
                    width: parent.width
                    height: valueHeight
                    backgroundVisible: (device.speedMode == 1)

                }
                ValueLabel {
                    varNameText: qsTr("Среднее потребление эл.эн")+ tswitch.lang
                    valueText: "99999"
                    unitsText: qsTr("квт-час/\сутки")+ tswitch.lang
                    width: parent.width
                    height: valueHeight
                    backgroundVisible: (device.speedMode != 1)


                }
                ValueLabel {
                    varNameText: qsTr("Итоговое потребление эл.эн")+ tswitch.lang
                    valueText: "99999"
                    unitsText: qsTr("квт-час")+ tswitch.lang
                    width: parent.width
                    height: 23
                    backgroundVisible: (device.speedMode == 1)

                }

            }
        }
    } //Rectangle
} // Item
