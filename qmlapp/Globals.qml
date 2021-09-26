pragma Singleton
import QtQuick 2.9
import QtQuick.Controls.Material 2.1

import dev.pump.station.gui 1.0

QtObject {
    property int textSize: 20

    property color mainViewLabelClr: "#000000"
    property font  mainViewLabelFnt: Qt.font({family: "Roboto", pixelSize: 18, bold: false})
    property font  mainViewValFnt: Qt.font({family: "Roboto", pixelSize: 24, bold: true})
    property font  mainViewUnitsFnt: Qt.font({family: "Roboto", pixelSize: 17, bold: false})

    property int   topHeaderHeight: 17
    property color topHeaderBackground: "#303030"

    property color topHeaderStartedBackground: "#ff109820"
    property color topHeaderText: "#000000"
    property color topHeaderNOVFCColor: "#bf1010"
    property color topHeaderNOVFCShadowColor: "#7f7f1010"
    property font  topHeaderFnt: Qt.font({family: "Roboto Condensed", pixelSize: 17, bold: false})
    property font  dateHeaderFnt: Qt.font({family: "Oxygen Mono", pixelSize: 14, bold: false})
    property font  timeHeaderFnt: Qt.font({family: "Oxygen Mono", pixelSize: 17, bold: true})

    property int   tableHeaderHeight: 14
    property color mainViewBackground:  gui.isGuiDark ? "#000000" : "#ffffff"

    property color processStatusBar: "#207f20"
    property color processStoppedStatusBar: "#d5d5d5"

    property color processStatusBarNOVFCColor: "#7f1010"
    property color processStatusBarText: "#ffffffff"
    property color processStoppedStatusBarText: "#ffffffff"
    property font  processStatusBarFnt: Qt.font({family: "Roboto Condensed", pixelSize: 16, bold: false})


    property color currentProfileStatusBarText: "#ffffffff"
    property color currentProfileStatusBar: "#303030"

    property font  currentProcessBarFnt: Qt.font({family: "Oxygen Mono", pixelSize: 15, bold: false})
    property font  currentProcessBarPrefixFnt: Qt.font({family: "Oxygen Mono", pixelSize: 15, bold: false})
    property font  currentProcessBarValueFnt: Qt.font({family: "Oxygen Mono", pixelSize: 17, bold: true})
    property font  currentProfileStatusBarFnt: Qt.font({family: "Oxygen Mono", pixelSize: 14, bold: false})

    property font  currentProfileStatusBarHeaderFnt: Qt.font({family: "Roboto Condensed", pixelSize: 16, bold: false})

    property font  graphLabelHeaderFnt: Qt.font({family: "Roboto Condensed", pixelSize: 16, bold: false})
    property font  graphLabelFnt: Qt.font({family: "Roboto Condensed", pixelSize: 17, bold: false})
    property font  graphLegendValueFnt: Qt.font({family: "Oxygen Mono", pixelSize: 18, bold: false})
    property font  graphLegendUnitsFnt: Qt.font({family: "Roboto Condensed", pixelSize: 18, bold: false})

    property font  dynamoLabelHeaderFnt: Qt.font({family: "Roboto Condensed", pixelSize: 14, bold: false})
    property font  dynamoLabelFnt: Qt.font({family: "Roboto Condensed", pixelSize: 15, bold: false})
    property font  dynamoLegendValueFnt: Qt.font({family: "Oxygen Mono", pixelSize: 17, bold: false})
    property font  dynamoLegendUnitsFnt: Qt.font({family: "Roboto Condensed", pixelSize: 16, bold: false})


    property color l2HeaderTitleBackground:  "#ffd0d0d0"

    property color headerText: "#ffffff" 
    property color headerBackground: "#000000" 

}
