#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtQml/qqml.h> // Nécessaire pour QML_ELEMENT
#include <QNetworkInterface> // Pour récupérer l'IP

class Backend : public QObject
{
    Q_OBJECT
    // Cette macro rend la classe visible dans QML automatiquement
    QML_ELEMENT

public:
    explicit Backend(QObject *parent = nullptr);

    // Q_INVOKABLE rend cette méthode appelable depuis le QML
    Q_INVOKABLE QString getDeviceIp();
};

#endif // BACKEND_H
