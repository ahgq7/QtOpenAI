#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply* get(const QNetworkRequest &request);
};

#endif // NETWORKMANAGER_H