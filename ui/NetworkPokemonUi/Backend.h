// Backend.h
#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtQml/qqml.h>
#include "networkPokemon.h" // Ta lib

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Backend(QObject *parent = nullptr);
    Q_INVOKABLE QString getDeviceIp();
    Q_INVOKABLE void saveSomething(); // Nouvelle méthode appelable du QML
    Q_INVOKABLE void saveSettings(QString name, QString port, QString maxConn, bool share, bool download);
    Q_INVOKABLE QVariantMap getConfig();

private:
    pokemon::Test m_pokemonEngine; // L'instance unique de ton moteur
};

#endif // BACKEND_H
