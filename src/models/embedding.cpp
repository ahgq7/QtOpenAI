#include "embedding.h"
#include "network/networkmanager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Embedding::Embedding(QObject *parent, NetworkManager* networkManager) : QObject(parent), m_networkManager(networkManager)
{

}

void Embedding::setApiKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

void Embedding::setModel(const QString &model)
{
    m_model = model;
}

void Embedding::getEmbedding(const QString &input)
{
    if (m_apiKey.isEmpty()) {
        emit error("API key is not set.");
        return;
    }

    QJsonObject json;
    json["model"] = m_model;
    json["input"] = input;

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.openai.com/v1/embeddings"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_apiKey).toUtf8());

     QByteArray requestData = QJsonDocument(json).toJson();
    if(m_reply) { //Önceki istek varsa iptal et
        m_reply->abort();
        m_reply->deleteLater();
    }
    m_reply = m_networkManager->post(request, requestData);
    connect(m_reply, &QNetworkReply::finished, this, &Embedding::handleReply);

}

void Embedding::handleReply()
{
    if (!m_reply) return;
    if (m_reply->error() == QNetworkReply::NoError) {
        QByteArray data = m_reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains("data")){
                QJsonArray dataArray = obj["data"].toArray();
                if(!dataArray.isEmpty()){
                    QJsonObject firstItem = dataArray[0].toObject();
                    if(firstItem.contains("embedding")){
                        QJsonArray embedding = firstItem["embedding"].toArray();
                         emit embeddingReady(embedding);
                         m_reply->deleteLater();
                         m_reply = nullptr;
                         return;
                    }
                }
            }
        }
         emit error("Unexpected JSON format.");

    }else{
        emit error(m_reply->errorString());
    }
    m_reply->deleteLater();
    m_reply = nullptr;
}