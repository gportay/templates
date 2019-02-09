#include <QtGlobal>
#include <QString>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QtWebView>
#include <QQuickItem>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QCoreApplication::setApplicationName("WebView");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("A simple WebViewer based on Qt WebView.");
    QCommandLineOption urlOption(QStringList() << "u" << "url",
                                 QCoreApplication::translate("main", "Set default URL."),
                                 QCoreApplication::translate("main", "url"));
    parser.addOption(urlOption);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    QtWebView::initialize();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    if (!window)
        return -1;

    QQuickItem *webView = window->findChild<QQuickItem *>("webview", Qt::FindDirectChildrenOnly);
    if (!webView)
        return -1;

    QString url = parser.value(urlOption);
    if (url.length())
        webView->setProperty("url", url);

    return app.exec();
}
