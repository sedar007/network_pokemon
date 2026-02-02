#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // <--- Indispensable pour injecter les objets C++ vers QML

#include "node.h"
#include "peermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Node networkNode;

    PeerModel model(&networkNode);

    engine.rootContext()->setContextProperty("myPeerModel", &model);
    engine.rootContext()->setContextProperty("networkNode", &networkNode);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Chargement de l'interface
    engine.loadFromModule("NetworkPokemonUi", "Main");

    return app.exec();
}
