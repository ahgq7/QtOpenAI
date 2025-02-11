#ifndef IMAGEGENERATION_H
#define IMAGEGENERATION_H

#pragma once
#include "qtopenai_global.h"

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkManager;

class QTOPENAI_EXPORT ImageGeneration : public QObject
{
    Q_OBJECT

public:
    ImageGeneration(QObject* parent, NetworkManager* networkManager);

    void setApiKey(const QString& apiKey);
    void setModel(const QString& model);
    void generateImage(const QString& prompt, const QString& size = "1024x1024", int n = 1, const QString& quality="standard", const QString& style = "vivid");


signals:
    void imageReady(const QList<QUrl>& imageUrls);
    void error(const QString& errorString);
private slots:
    void handleReply();
private:
     NetworkManager* m_networkManager;
    QString m_apiKey;
    QString m_model = "dall-e-3";
    QNetworkReply* m_reply = nullptr;
};
#endif // IMAGEGENERATION_H
