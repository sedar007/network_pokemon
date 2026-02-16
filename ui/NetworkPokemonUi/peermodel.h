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

private:
    QList<Peer> m_peers;
    Node* m_node;
};

#endif // PEERMODEL_H
