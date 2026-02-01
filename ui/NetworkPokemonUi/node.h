//
// Created by Adjy Sedar Desir on 31/01/2026.
//

#ifndef NETWORKPOKEMON_NODE_H
#define NETWORKPOKEMON_NODE_H

#include <QObject>
#include <QtQml/qqml.h>
#include "networkPokemon.h"

class Node  : public QObject{

    Q_OBJECT
    QML_ELEMENT

    public:
        explicit Node(QObject *parent = nullptr);
        Q_INVOKABLE void save_node_infos(QString name, QString port, QString maxConn, bool share, bool download);
        Q_INVOKABLE QVariantMap get_node_infos();
        Q_INVOKABLE QString get_ip();


    private:
        pokemon::Node m_node;
};


#endif //NETWORKPOKEMON_NODE_H

