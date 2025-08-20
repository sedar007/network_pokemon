#pragma once
#include <QObject>
#include <QString>
#include "pch.h" // contient pokemon::Test

class TestWrapper : public QObject {
    Q_OBJECT
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit TestWrapper(QObject* parent = nullptr) : QObject(parent) {}

    int age() { return test.getAge(); }
    QString name() { return QString::fromStdString(test.getLocalIp()); }

public slots:
            void setAge(int a) {
        if (a != test.getAge()) {
            test.setAge(a);
            emit ageChanged();
        }
    }

    void setName(const QString &n) {
        if (n.toStdString() != test.getName()) {
            test.setName(n.toStdString());
            emit nameChanged();
        }
    }

    signals:
            void ageChanged();
    void nameChanged();

private:
    pokemon::Test test;
};
