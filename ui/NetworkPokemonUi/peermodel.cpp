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
    roles[NameRole] = "name";
    roles[IpRole] = "ip";
    roles[StatusRole] = "status";
    roles[CountRole] = "count";
    roles[PingRole] = "ping";
    roles[LastSeenRole] = "lastSeen";
    return roles;
}

void PeerModel::refreshPeers()
{
    if (!m_node) return; // Sécurité si m_node est null

    beginResetModel();
    m_peers.clear();

    // 1. Récupérer la liste brute depuis Node
    // Node retourne une QVariantList, chaque élément est supposé être une QVariantMap
    QVariantList rawList = m_node->get_node_list();

    // 2. Convertir les données
    for (const QVariant &item : rawList) {
        QVariantMap map = item.toMap();

        // On crée un objet Peer en extrayant les clés de la map
        // (Assurez-vous que les clés correspondent à ce que Node::get_node_list renvoie)
        Peer newPeer;
        newPeer.name = map.value("name", "Inconnu").toString();
        newPeer.ip = map.value("ip", "0.0.0.0").toString();
        newPeer.status = map.value("status").toInt() == 1 ? "online" : "Hors ligne";
        newPeer.count = map.value("count", "0 Pokémon").toString();
        newPeer.ping = map.value("ping", "-").toString();
        newPeer.lastSeen = "À l'instant";

        m_peers.append(newPeer);
    }

    endResetModel();
}
