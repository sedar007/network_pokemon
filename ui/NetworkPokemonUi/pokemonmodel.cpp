#include "pokemonmodel.h"

PokemonModel::PokemonModel(Node* node, QObject *parent)
    : QAbstractListModel(parent), m_node(node)
{
    refreshPokemons();
}

int PokemonModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_pokemons.size();
}

QVariant PokemonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_pokemons.size())
        return QVariant();

    const PokemonItem &item = m_pokemons[index.row()];

    switch (role) {
    case NameRole:   return item.name;
    case NumberRole: return item.pNumber;
    case TypeRole:   return item.type;
    case SizeRole:   return item.size;
    case ImgUrlRole: return item.imgUrl;
    default:         return QVariant();
    }
}

QHash<int, QByteArray> PokemonModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[NumberRole] = "pNumber";
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    roles[ImgUrlRole] = "imgUrl";
    return roles;
}

void PokemonModel::refreshPokemons()
{
    if (!m_node) return;

    beginResetModel();
    m_pokemons.clear();

    QVariantList rawList = m_node->get_pokemon_list();

    for (const QVariant &var : rawList) {
        QVariantMap map = var.toMap();

        PokemonItem item;
        item.name = map.value("name", "Unknown").toString();
        item.pNumber = map.value("pNumber", "???").toString();
        item.type = map.value("type", "Normal").toString();
        item.size = map.value("size", "0 MB").toString();
        item.imgUrl = map.value("imgUrl", "").toString();

        m_pokemons.append(item);
    }

    endResetModel();
}
