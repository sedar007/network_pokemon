#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // <--- Indispensable pour injecter les objets C++ vers QML
#include <QStandardPaths>

#include "node.h"
#include "peermodel.h"
#include "pokemonmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    QString writablePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(writablePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    std::string storagePath = (writablePath + QDir::separator()).toStdString();

    QQmlApplicationEngine engine;
    pokemon::peer_registry peerRegistry;
    pokemon::image_repository imageRepo(storagePath);

    Node networkNode(peerRegistry, imageRepo);

    PeerModel model(&networkNode);
    PokemonModel pokeModel(&networkNode);

    engine.rootContext()->setContextProperty("myPeerModel", &model);
    engine.rootContext()->setContextProperty("myPokemonModel", &pokeModel);
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
