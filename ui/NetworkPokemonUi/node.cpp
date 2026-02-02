//
// Created by Adjy Sedar Desir on 31/01/2026.
//

#include "node.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Node::Node(QObject *parent)
    : QObject{parent}
{
    QString writablePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(writablePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString fullPath = writablePath + QDir::separator();
    m_node.initialized(fullPath.toStdString(), "", "");
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
    map["maxConnections"] = 10;//QString::number(config.maxConnections);
    map["autoShare"] = 0;//config.autoShare;
    map["autoDownload"] = 0;//config.autoDownload;

    qDebug() << "Config chargée depuis le disque.";
    return map;
}

QVariantList Node::get_node_list() {
    QVariantList result;
    auto nodes = m_node.get_node_list();

    for (const auto& nodeInfo : nodes) {
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

    m_node.add_peer(
        name.toStdString(),
        port.toStdString()
    );

    qDebug() << "Configuration sauvegardée pour :" << name;
}


