//
// Created by Adjy Sedar Desir on 31/01/2026.
//

#include "node.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Node::Node(pokemon::peer_registry& registry, pokemon::image_repository& image_repository, QObject *parent)
    : QObject{parent}, m_node(registry, image_repository)
{
    m_node.initialized();
}

void Node::save_node_infos(QString name, QString port, QString maxConn, bool share, bool download)
{
    bool portOk = false;
    bool maxConnOk = false;
    const int portValue = port.toInt(&portOk);
    const int maxConnValue = maxConn.toInt(&maxConnOk);

    m_node.set_node_info(
        name.toStdString(),
        portOk ? portValue : 0,
        maxConnOk ? maxConnValue : pokemon::Node_Info::DEFAULT_MAX_CONNECTIONS,
        share,
        download
    );

    qDebug() << "Configuration sauvegardée pour :" << name;
}


QVariantMap Node::get_node_infos()
{
    QVariantMap map;

    auto config = m_node.get_node_info();
    map["nodeName"] = QString::fromStdString(config.get_name().data());
    map["port"] = QString::number(config.get_port());
    map["maxConnections"] = config.get_max_connections();
    map["autoShare"] = config.get_auto_share();
    map["autoDownload"] = config.get_auto_download();

    qDebug() << "Config chargée depuis le disque.";
    return map;
}

QVariantList Node::get_node_list() {
    QVariantList result;
    auto nodes = m_node.get_node_list();

    for (const auto& nodeInfo : nodes) {
        if (nodeInfo.get_id() == m_node.get_node_info().get_id()) {
            continue;
        }
        QVariantMap nodeMap;
        nodeMap["name"] = QString::fromStdString(std::string(nodeInfo.get_name()));
        nodeMap["port"] = QString::number(nodeInfo.get_port());
        nodeMap["ip"] = QString::fromStdString(std::string(nodeInfo.get_ip()));
        nodeMap["status"] = nodeInfo.is_connected();
        result.append(nodeMap);
    }

    return result;
}


QString Node::get_ip()
{
    return QString::fromStdString(m_node.get_ip());
}

bool Node::is_p2p_active()
{
    // Pas de mécanisme actuel pour remonter un échec de bind/écoute du serveur ;
    // ceci reflète au moins que le nœud a un port assigné après initialisation,
    // au lieu d'un simple indicateur toujours vert.
    return m_node.get_node_info().get_port() > 0;
}

void Node::add_peer(QString name, QString ip, QString port)
{
    bool portOk = false;
    const int portValue = port.toInt(&portOk);

    if (portOk && portValue > 0) {
        m_node.add_peer(ip.toStdString(), static_cast<in_port_t>(portValue));
    } else {
        m_node.add_new_peer(ip.toStdString());
    }

    qDebug() << "Ajout du peer :" << name << ip << port;
}


QVariantList Node::get_pokemon_list() {
    QVariantList result;

    auto images = m_node.get_image_list();
    // get_node_info() renvoie un Node_Info temporaire par valeur : on copie l'id
    // dans un std::string propriétaire, sinon myId (string_view) pointerait dans
    // de la mémoire déjà libérée dès la fin de cette instruction.
    const std::string myId(m_node.get_node_info().get_id());

    for (const auto& img : images) {
        QVariantMap map;

        map["name"] = QString::fromStdString(img.get_name().data());
        map["pNumber"] = "";
        map["type"] = "";
        map["size"] = QString::fromStdString(img.get_size().data());
        map["sizeUnit"] = QString::fromStdString(img.get_size_unit().data());
        map["hash"] = QString::fromStdString(std::string(img.get_hash()));
        map["isMine"] = (img.get_owner() == myId);

        std::string b64 = m_node.get_picture(img);
        map["imgUrl"] = QString::fromStdString(b64);

        result.append(map);
    }

    return result;
}

void Node::add_pokemon(QString name, QString filePath)
{
    QUrl url(filePath);
    QString localPath = url.isLocalFile() ? url.toLocalFile() : filePath;

    qDebug() << "Ajout de l'image :" << localPath;

    m_node.add_pokemon(name.toStdString(), localPath.toStdString());
}

void Node::remove_pokemon(QString hash)
{
    qDebug() << "Suppression de l'image :" << hash;

    m_node.remove_pokemon(hash.toStdString());
}





