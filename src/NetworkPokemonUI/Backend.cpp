#include "Backend.h"
#include "networkPokemon.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{}

QString Backend::getDeviceIp()
{
    pokemon::Test test;
    return QString::fromStdString(test.getIp());
    //return "10.1.1.100";
}
