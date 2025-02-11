#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {}

QNetworkReply *NetworkManager::post(const QNetworkRequest &request, const QByteArray &data) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    return manager->post(request, data);
}

QNetworkReply* NetworkManager::get(const QNetworkRequest &request)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    return manager->get(request);
}