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
    case IsMineRole: return item.isMine;
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
    roles[IsMineRole] = "isMine";
    return roles;
}

void PokemonModel::refreshPokemons()
{
    if (!m_node) return;

    beginResetModel();
    m_pokemons.clear();

    QVariantList rawList = m_node->get_pokemon_list();

    int i = 0;
    for (const QVariant &var : rawList) {
        QVariantMap map = var.toMap();

        PokemonItem item;
        item.name = map.value("name", "Unknown").toString();
        item.pNumber = map.value("pNumber", "???").toString();
        item.type = map.value("type", "Normal").toString();
        item.size = map.value("size", "0 MB").toString();
        item.imgUrl = map.value("imgUrl", "").toString();

        if (i++ % 2 == 0) {
            item.isMine = map.value("isMine", false).toBool();
        } else {
            item.isMine = map.value("isMine", true).toBool();
        }
        m_pokemons.append(item);
    }

    endResetModel();
}

void PokemonModel::addPokemon(const QString &name, const QString &filePath)
{
    QUrl url(filePath);
    QString localPath = url.isLocalFile() ? url.toLocalFile() : filePath;

    qDebug() << "Ajout de l'image :" << localPath;

    if (m_node) {
        m_node->add_pokemon(name, localPath);
    }

    refreshPokemons();
}

void PokemonModel::removePokemon(int index)
{
    if (index < 0 || index >= m_pokemons.size())
        return;

    // Indispensable pour que le QML sache qu'une ligne va disparaître
    beginRemoveRows(QModelIndex(), index, index);

    // 1. Supprimer de la liste en mémoire
    // (Optionnel : Supprimer aussi le fichier physique sur le disque ici via std::filesystem::remove)
    m_pokemons.removeAt(index);

    endRemoveRows();
}

void PokemonModel::savePokemon(int index, const QString &destinationPath)
{
  /*  if (index < 0 || index >= m_pokemons.size()) return;

    QString cleanPath = QUrl(destinationPath).toLocalFile();
    if (cleanPath.isEmpty()) cleanPath = destinationPath;

    QString base64Data = m_pokemons[index].imgUrl;


    if (base64Data.startsWith("data:image")) {

        int commaIndex = base64Data.indexOf(",");
        if (commaIndex != -1) {
            base64Data = base64Data.mid(commaIndex + 1);
        }

        QByteArray data = QByteArray::fromBase64(base64Data.toLatin1());
        QFile file(cleanPath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            file.close();
            qDebug() << "Image téléchargée avec succès :" << cleanPath;
        }
    } else {
        QFile::copy(base64Data, cleanPath);
    }*/
}
