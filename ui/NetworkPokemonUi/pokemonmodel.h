#ifndef POKEMONMODEL_H
#define POKEMONMODEL_H

#include <QAbstractListModel>
#include "node.h"

struct PokemonItem {
    QString name;
    QString pNumber;
    QString type;
    QString size;
    QString imgUrl;
};

class PokemonModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PokemonModel(Node* node, QObject *parent = nullptr);

    enum PokemonRoles {
        NameRole = Qt::UserRole + 1,
        NumberRole,
        TypeRole,
        SizeRole,
        ImgUrlRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void refreshPokemons();
    Q_INVOKABLE int refreshFrequency() const { return 5000; } // 5 seconds


private:
    Node* m_node;
    QList<PokemonItem> m_pokemons;
};

#endif // POKEMONMODEL_H
