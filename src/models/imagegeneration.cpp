#include "imagegeneration.h"
#include "network/networkmanager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>

ImageGeneration::ImageGeneration(QObject* parent, NetworkManager* networkManager) :
    QObject(parent), m_networkManager(networkManager)
{

}

void ImageGeneration::setApiKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

void ImageGeneration::setModel(const QString &model)
{
    m_model = model;
}

void ImageGeneration::generateImage(const QString &prompt, const QString& size, int n, const QString &quality, const QString &style)
{
   if (m_apiKey.isEmpty()) {
        emit error("API key is not set.");
        return;
    }

    QJsonObject json;
    json["model"] = m_model;
    json["prompt"] = prompt;
    json["n"] = n;
    json["size"] = size;
    json["quality"] = quality;
    json["style"] = style;


    QNetworkRequest request;
    request.setUrl(QUrl("https://api.openai.com/v1/images/generations"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_apiKey).toUtf8());
    QByteArray requestData = QJsonDocument(json).toJson();

    if(m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
    }

    m_reply = m_networkManager->post(request, requestData);
    connect(m_reply, &QNetworkReply::finished, this, &ImageGeneration::handleReply);
}

void ImageGeneration::handleReply()
{
    if (!m_reply) return;

   if (m_reply->error() == QNetworkReply::NoError) {
        QByteArray data = m_reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if(doc.isObject()){
            QJsonObject obj = doc.object();
            if(obj.contains("data")){
                QJsonArray dataArray = obj["data"].toArray();
                QList<QUrl> urls;
                for(const QJsonValue& value : dataArray) {
                    QJsonObject imageObj = value.toObject();
                    if (imageObj.contains("url")) {
                        urls.append(QUrl(imageObj["url"].toString()));
                    }
                }
                emit imageReady(urls);
                m_reply->deleteLater();
                m_reply = nullptr;
                return;

            }
        }
          emit error("Unexpected JSON format.");
    }
    else {
        emit error(m_reply->errorString());
    }
    m_reply->deleteLater();
    m_reply = nullptr;
}