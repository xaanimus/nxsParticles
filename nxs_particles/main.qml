import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import MainLib 1.0

Window {
    id: page
    visible: true
    width: 1600
    height: 800

    MainView {
        anchors.fill: parent
        focus: true
    }
}
