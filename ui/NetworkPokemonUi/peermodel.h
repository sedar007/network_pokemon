#ifndef PEERMODEL_H
#define PEERMODEL_H

#include <QAbstractListModel>
#include <QObject>
// Il est préférable d'utiliser une déclaration anticipée (forward declaration) ici
// pour éviter les inclusions circulaires.
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

    // MODIFICATION : Le constructeur prend maintenant un pointeur vers Node
    explicit PeerModel(Node* node, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Cette fonction ira chercher les vraies données
    Q_INVOKABLE void refreshPeers();

private:
    QList<Peer> m_peers;
    Node* m_node; // Pointeur vers l'instance de Node
};

#endif // PEERMODEL_H
