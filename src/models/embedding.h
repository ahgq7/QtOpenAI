#ifndef EMBEDDING_H
#define EMBEDDING_H

#pragma once
#include "qtopenai_global.h"

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkManager;

class QTOPENAI_EXPORT Embedding : public QObject
{
    Q_OBJECT
public:
    Embedding(QObject* parent, NetworkManager* networkManager);

    void setApiKey(const QString& apiKey);
    void setModel(const QString& model);
    void getEmbedding(const QString &input);

signals:
    void embeddingReady(const QJsonArray& embedding);
    void error(const QString &errorString);
private slots:
    void handleReply();
private:
    NetworkManager* m_networkManager;
    QString m_apiKey;
    QString m_model = "text-embedding-ada-002";
    QNetworkReply* m_reply = nullptr;
};

#endif // EMBEDDING_H
