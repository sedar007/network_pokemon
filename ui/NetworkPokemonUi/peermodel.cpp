#include "peermodel.h"
#include "node.h" // On inclut le header complet ici pour avoir accès aux méthodes

// MODIFICATION : On initialise le pointeur m_node
PeerModel::PeerModel(Node* node, QObject *parent)
    : QAbstractListModel(parent), m_node(node)
{
    // On charge les données réelles dès le démarrage
    refreshPeers();
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_peers.size();
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

QHash<int, QByteArray> PeerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = NAME_MODEL.data();
    roles[IpRole] = IP_ROLE.data();
    roles[StatusRole] = STATUS_ROLE.data();
    roles[CountRole] = COUNT_ROLE.data();
    roles[PingRole] = PING_ROLE.data();
    roles[LastSeenRole] = LastSeen_ROLE.data();
    return roles;
}

void PeerModel::refreshPeers()
{
    if (!m_node) return; // Sécurité si m_node est null

    beginResetModel();
    m_peers.clear();

    // TODO add cache..... call the api after x seconds
    // refresh time 5 seconds ???

    QVariantList rawList = m_node->get_node_list();


    for (const QVariant &item : rawList) {
        QVariantMap map = item.toMap();

        Peer newPeer;
        newPeer.name = map.value(NAME_MODEL.data(), "Inconnu").toString();
        newPeer.ip = map.value(IP_ROLE.data(), "0.0.0.0").toString();
        newPeer.status = map.value(STATUS_ROLE.data()).toInt() == 1 ? "online" : "Hors ligne";
        newPeer.count = map.value(COUNT_ROLE.data(), "0 Pokémon").toString();
        newPeer.ping = map.value(PING_ROLE.data(), "-").toString();
        newPeer.lastSeen = "À l'instant";

        m_peers.append(newPeer);
    }

    endResetModel();
}


int PeerModel::peers_count()
{
    if (!m_node) return 0;
    return m_peers.size();
}
