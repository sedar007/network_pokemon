#ifndef PEERMODEL_H
#define PEERMODEL_H

#include <QAbstractListModel>
#include <QtQml/qqml.h>
#include <vector>
#include <QRandomGenerator>

// Structure simple pour stocker les données
struct Peer {
    QString name;
    QString ip;
    QString status;   // "online", "sync", "offline"
    QString count;    // ex: "156 Pokémon"
    QString ping;     // ex: "12ms"
    QString lastSeen; // ex: "Vu à l'instant"
};

class PeerModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT // Rend la classe utilisable directement dans le QML

public:
    explicit PeerModel(QObject *parent = nullptr);

    // Méthodes obligatoires pour QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Méthode pour ajouter un peer depuis le C++ (utile pour ton réseau P2P)
    Q_INVOKABLE void addPeer(const QString &name, const QString &ip, const QString &status);

    Q_INVOKABLE void refreshPeers();

    // Enum pour mapper les champs C++ vers le QML
    enum PeerRoles {
        NameRole = Qt::UserRole + 1,
        IpRole,
        StatusRole,
        CountRole,
        PingRole,
        LastSeenRole
    };

private:
    std::vector<Peer> m_peers;
    QString getRandomIp();
};

#endif // PEERMODEL_H
