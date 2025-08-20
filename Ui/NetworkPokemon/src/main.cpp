#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QApplication>
#include "TestWrapper.h"



int main(int argc, char *argv[])
{
     QGuiApplication app(argc, argv);
   //  QApplication::setApplicationName("Network Pokemon");

     QQmlApplicationEngine engine;


    TestWrapper testWrapper;
    testWrapper.setAge(10);
    testWrapper.setName("Charmander");

    engine.rootContext()->setContextProperty("testObj", &testWrapper);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("NetworkPokemon", "Main");

    return app.exec();
}

