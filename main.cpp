#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "MyVideoFilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<MyVideoFilter>("MyVideoFilterLib", 1, 0, "MyVideoFilter");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
