#include "chatcompletion.h"
#include "network/networkmanager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


ChatCompletion::ChatCompletion(QObject *parent, NetworkManager* networkManager)
    : QObject(parent), m_networkManager(networkManager) {}


void ChatCompletion::setApiKey(const QString &apiKey) {
    m_apiKey = apiKey;
}

void ChatCompletion::setModel(const QString &model) {
    m_model = model;
}

void ChatCompletion::setTemperature(double temperature) {
    m_temperature = temperature;
}

void ChatCompletion::setMaxTokens(int maxTokens)
{
    m_maxTokens = maxTokens;
}

void ChatCompletion::setEndPoint(const QString &newEndPoint) {
    m_endPoint = newEndPoint;
}

void ChatCompletion::send(const QJsonArray &messages) {
    if (m_apiKey.isEmpty()) {
        emit error("API key is not set.");
        return;
    }

    QJsonObject json;
    json["model"] = m_model;
    json["messages"] = messages;
    json["temperature"] = m_temperature;
    json["max_tokens"] = m_maxTokens;

    QNetworkRequest request;
    request.setUrl(QUrl(m_endPoint));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_apiKey).toUtf8());

    QByteArray requestData = QJsonDocument(json).toJson();

    if(m_reply) { //Önceki istek varsa iptal et
        m_reply->abort();
        m_reply->deleteLater();
    }
    m_reply = m_networkManager->post(request, requestData);

    connect(m_reply, &QNetworkReply::finished, this, &ChatCompletion::handleReply);
}


void ChatCompletion::handleReply() {

     if (!m_reply) return;

    if (m_reply->error() == QNetworkReply::NoError) {
        QByteArray data = m_reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.contains("choices")) {
                QJsonArray choices = obj["choices"].toArray();
                if (!choices.isEmpty()) {
                    QString content = choices[0].toObject()["message"].toObject()["content"].toString();
                    emit ready(content);
                    m_reply->deleteLater();
                     m_reply = nullptr;
                    return;
                }
            }
        }
        emit error("Unexpected JSON format.");
    } else {
        emit error(m_reply->errorString());
    }
    m_reply->deleteLater();
    m_reply = nullptr;
}
