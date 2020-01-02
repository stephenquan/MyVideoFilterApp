#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "NullVideoFilter.h"
#include "GreyScaleVideoFilter.h"
#include "ShowCornersVideoFilter.h"
#include "BlueToRedVideoFilter.h"
#include "EditCenterVideoFilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType< NullVideoFilter >("MyVideoFilterLib", 1, 0, "NullVideoFilter" );
    qmlRegisterType< GreyScaleVideoFilter >("MyVideoFilterLib", 1, 0, "GreyScaleVideoFilter" );
    qmlRegisterType< EditCenterVideoFilter >("MyVideoFilterLib", 1, 0, "EditCenterVideoFilter" );
    qmlRegisterType< ShowCornersVideoFilter >("MyVideoFilterLib", 1, 0, "ShowCornersVideoFilter" );
    qmlRegisterType< BlueToRedVideoFilter >("MyVideoFilterLib", 1, 0, "BlueToRedVideoFilter" );
    engine.load( QUrl( QLatin1String( "qrc:/main.qml" ) ) );

    return app.exec();
}
