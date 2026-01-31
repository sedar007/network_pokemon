#include "peerModel.h"

PeerModel::PeerModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // Ajout de fausses données pour tester l'interface
    m_peers = {
        {"Ash-Nodeeeeee", "192.168.1.102", "online", "156 Pokémon", "12ms", "Vu à l'instant"},
        {"Misty-Server", "192.168.1.105", "online", "89 Pokémon", "8ms", "Vu à l'instant"},
        {"Brock-Node", "192.168.1.110", "sync", "234 Pokémon", "45ms", "Vu il y a 22h"},
        {"Gary-Server", "192.168.1.115", "offline", "178 Pokémon", "-", "Vu il y a 23h"},
        {"Prof-Oak", "192.168.1.120", "online", "721 Pokémon", "15ms", "Vu à l'instant"}
    };
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_peers.size());
}

QVariant PeerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_peers.size())
        return QVariant();

    const Peer &peer = m_peers[index.row()];

    switch (role) {
    case NameRole:     return peer.name;
    case IpRole:       return peer.ip;
    case StatusRole:   return peer.status;
    case CountRole:    return peer.count;
    case PingRole:     return peer.ping;
    case LastSeenRole: return peer.lastSeen;
    default:           return QVariant();
    }
}

void PeerModel::refreshPeers()
{
    // 1. On prévient la vue qu'on va tout changer
    beginResetModel();

    // 2. On vide la liste actuelle
    m_peers.clear();

    // 3. Simulation : On génère un nombre aléatoire de peers (entre 2 et 8)
    int count = QRandomGenerator::global()->bounded(2, 9);

    // Quelques noms pour la démo
    QStringList names = {"Ash-Node", "Misty-Server", "Brock-Node", "Gary-Server", "Jessie-PC", "James-Mac", "Oak-Lab", "Giovanni-Base"};

    for (int i = 0; i < count; ++i) {
        // Choix aléatoire des propriétés
        QString name = names[QRandomGenerator::global()->bounded(names.size())];
        QString ip = QString("192.168.1.%1").arg(QRandomGenerator::global()->bounded(100, 200));

        int statusRand = QRandomGenerator::global()->bounded(3);
        QString status = (statusRand == 0) ? "online" : (statusRand == 1) ? "sync" : "offline";

        QString ping = (status == "offline") ? "-" : QString("%1ms").arg(QRandomGenerator::global()->bounded(5, 150));
        QString pokemonCount = QString("%1 Pokémon").arg(QRandomGenerator::global()->bounded(50, 900));

        m_peers.push_back({name, ip, status, pokemonCount, ping, "Vu à l'instant"});
    }

    // 4. On prévient la vue que c'est fini
    endResetModel();
}

QHash<int, QByteArray> PeerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    // C'est ici qu'on définit les noms utilisables dans le QML (ex: model.name)
    roles[NameRole] = "name";
    roles[IpRole] = "ip";
    roles[StatusRole] = "status";
    roles[CountRole] = "count";
    roles[PingRole] = "ping";
    roles[LastSeenRole] = "lastSeen";
    return roles;
}

void PeerModel::addPeer(const QString &name, const QString &ip, const QString &status)
{
    beginInsertRows(QModelIndex(), m_peers.size(), m_peers.size());
    m_peers.push_back({name, ip, status, "0 Pokémon", "-", "Nouveau"});
    endInsertRows();
}
