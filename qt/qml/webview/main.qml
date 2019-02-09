import QtQuick 2.8
import QtQuick.Window 2.2
import QtWebView 1.1

Window {
    visible: true
    width: 640
    height: 480
    title: webview.url

    WebView {
        objectName: "webview"
        id: webview;
        anchors.fill: parent
        url: "https://www.github.com/gportay/templates"
    }
}
