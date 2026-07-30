#ifndef PEERMODEL_H
#define PEERMODEL_H

#include <QAbstractListModel>
#include <QObject>
class Node;

struct Peer {
    QString name;
    QString ip;
    QString status;
    QString count;
    QString ping;
    QString lastSeen;
};

class PeerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum PeerRoles {
        NameRole = Qt::UserRole + 1,
        IpRole,
        StatusRole,
        CountRole,
        PingRole,
        LastSeenRole
    };

    explicit PeerModel(Node* node, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void refreshPeers();
    Q_INVOKABLE int peers_count();
    Q_INVOKABLE int online_peers_count();

private:
    QList<Peer> m_peers;
    Node* m_node;


    static constexpr std::string_view NAME_MODEL = "name";
    static constexpr std::string_view IP_ROLE = "ip";
    static constexpr std::string_view STATUS_ROLE = "status";
    static constexpr std::string_view COUNT_ROLE = "count";
    static constexpr std::string_view PING_ROLE = "ping";
    static constexpr std::string_view LastSeen_ROLE = "lastSeen";
};

#endif // PEERMODEL_H
