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
    port;
    maxConn;
    share;
    download;
    m_node.set_node_info(
        name.toStdString()
    );

    qDebug() << "Configuration sauvegardée pour :" << name;
}


QVariantMap Node::get_node_infos()
{
    QVariantMap map;

    auto config = m_node.get_node_info();
    map["nodeName"] = QString::fromStdString(config.get_name().data());
    map["port"] = QString::number(config.get_port());
    map["maxConnections"] = 10;
    map["autoShare"] = 0;
    map["autoDownload"] = 0;

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
        nodeMap["status"] = QString::number(nodeInfo.get_isConnected());
        result.append(nodeMap);
    }

    return result;
}


QString Node::get_ip()
{
    return QString::fromStdString(m_node.get_ip());
}

void Node::add_peer(QString name, QString port)
{

    m_node.add_new_peer(
        name.toStdString(),
        port.toStdString()
    );

    qDebug() << "Configuration sauvegardée pour :" << name;
}


QVariantList Node::get_pokemon_list() {
    QVariantList result;

    auto images = m_node.get_image_list();

    for (const auto& img : images) {
        QVariantMap map;

        map["name"] = QString::fromStdString(img.get_name().data());
        map["pNumber"] = "";
        map["type"] = "";
        map["size"] = QString::fromStdString(img.get_size().data());
        map["sizeUnit"] = QString::fromStdString(img.get_size_unit().data());

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

void Node::remove_pokemon(QString name, QString filePath)
{
    QUrl url(filePath);
    QString localPath = url.isLocalFile() ? url.toLocalFile() : filePath;

    qDebug() << "Ajout de l'image :" << localPath;

    m_node.add_pokemon(name.toStdString(), localPath.toStdString());
}





