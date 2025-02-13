#ifndef CHATCOMPLETION_H
#define CHATCOMPLETION_H

#pragma once
#include "qtopenai_global.h"

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetworkManager; // Forward declaration

class QTOPENAI_EXPORT ChatCompletion : public QObject {
    Q_OBJECT

public:
    explicit ChatCompletion(QObject *parent, NetworkManager* networkManager);

    void setApiKey(const QString &apiKey);
    void setModel(const QString &model);
    void setTemperature(double temperature);
    void setMaxTokens(int maxTokens);
    void setEndPoint(const QString &newEndPoint);
    void send(const QJsonArray &messages);

signals:
    void ready(const QString &result);
    void error(const QString &errorString);

private slots:
    void handleReply();

private:
    NetworkManager* m_networkManager;
    QString m_apiKey;
    QString m_model = "gpt-4o";
    QString m_endPoint = "https://api.openai.com/v1/chat/completions";
    double m_temperature = 0.7;
    int m_maxTokens = 1024;
    QNetworkReply* m_reply = nullptr;

};

#endif // CHATCOMPLETION_H
