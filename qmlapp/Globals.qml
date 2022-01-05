pragma Singleton
import QtQuick 2.9
import QtQuick.Controls.Material 2.1

QtObject {
    property int textSize: 20

    property color mainViewLabelClr: "#000000"
    property font  mainViewLabelFnt: Qt.font({family: "Roboto", pixelSize: 18, bold: false})
    property font  mainViewValFnt: Qt.font({family: "Roboto", pixelSize: 24, bold: true})
    property font  mainViewUnitsFnt: Qt.font({family: "Roboto", pixelSize: 17, bold: false})

    property int   topHeaderHeight: 17
    property color topHeaderBackground: "#303030"

    property color topHeaderStartedBackground: "#ff109820"
    property color topHeaderText: "#ffffff"
    property font  topHeaderFnt: Qt.font({family: "Roboto Condensed", pixelSize: 17, bold: false, color: "#ffffff"})
    property font  dateHeaderFnt: Qt.font({family: "Oxygen Mono", pixelSize: 14, bold: false})
    property font  timeHeaderFnt: Qt.font({family: "Oxygen Mono", pixelSize: 17, bold: true})

    property int   tableHeaderHeight: 14
    property color mainViewBackground: "#7f7f7f"

    property color headerText: "#ffffff" 
    property color headerBackground: "#000000"
}
