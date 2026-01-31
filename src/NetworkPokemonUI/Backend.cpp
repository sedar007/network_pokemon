#include "Backend.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    // 1. Récupérer le chemin autorisé sur iOS
    QString writablePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // 2. Créer le dossier s'il n'existe pas
    QDir dir(writablePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // --- CORRECTION MAJEURE ICI ---
    // Tu dois configurer le chemin sur TON instance membre 'm_pokemonEngine'
    // Sinon, elle ne sait pas où écrire !
    m_pokemonEngine.setStoragePath(writablePath.toStdString());

    qDebug() << "Backend initialisé. Dossier :" << writablePath;
}

QString Backend::getDeviceIp()
{
    // Note : Idéalement, utilise m_pokemonEngine ici aussi au lieu d'en recréer un
    return QString::fromStdString(m_pokemonEngine.getIp());
}

void Backend::saveSomething()
{
    m_pokemonEngine.savePeerData("Test manuel");
}

void Backend::saveSettings(QString name, QString port, QString maxConn, bool share, bool download)
{
    // Appel à ton moteur (qui a maintenant le bon chemin !)
    m_pokemonEngine.saveConfig(
        name.toStdString(),
        port.toInt(),
        maxConn.toInt(),
        share,
        download
    );

    qDebug() << "Configuration sauvegardée pour :" << name;
}

QVariantMap Backend::getConfig()
{
    // Récupère la config (std::optional)
    auto configOptional = m_pokemonEngine.getConfig();

    QVariantMap map;

    if (configOptional.has_value()) {
        // CAS 1 : Fichier trouvé
        auto config = configOptional.value();
        map["nodeName"] = QString::fromStdString(config.nodeName);
        map["port"] = QString::number(config.port);
        map["maxConnections"] = QString::number(config.maxConnections);
        map["autoShare"] = config.autoShare;
        map["autoDownload"] = config.autoDownload;

        qDebug() << "Config chargée depuis le disque.";
    }
    else {
        // --- CORRECTION 2 : CAS 2 (Premier lancement) ---
        // Si pas de fichier, on renvoie des valeurs par défaut pour que le QML s'affiche
        qDebug() << "Aucune config trouvée (premier lancement). Utilisation des défauts.";

        map["nodeName"] = "Nouveau Node";
        map["port"] = "8080";
        map["maxConnections"] = "10";
        map["autoShare"] = true;
        map["autoDownload"] = false;
    }

    return map;
}
