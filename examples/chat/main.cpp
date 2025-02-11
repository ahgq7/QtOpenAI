#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "qtopenai.h"
#include "models/chatcompletion.h"
#include "models/embedding.h"
#include "models/imagegeneration.h"

bool getUserInput(QString &userInput) {
    std::cout << "User: ";
    std::string input;
    if (!std::getline(std::cin, input)) {
        return false;
    }
    if (input == "exit") {
        return false;
    }
    userInput = QString::fromStdString(input);
    return true;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QtOpenAI openai;
    openai.setApiKey("YOUR_API_KEY"); // Buraya API anahtarınızı girin.

    ChatCompletion *chat = openai.chat();
    Embedding* embedding = openai.embeddings();
    ImageGeneration* imageGen = openai.image();

    // Chat Completion Example
    QJsonArray initialMessages;
    initialMessages.append(QJsonObject{{"role", "system"}, {"content", "You are a helpful assistant."}});
    initialMessages.append(QJsonObject{{"role", "user"}, {"content", "Merhaba, nasılsın?"}});

     //İlk mesajı gönder
    chat->send(initialMessages);


    QObject::connect(chat, &ChatCompletion::ready, [&](const QString &result) {
        qDebug() << "Assistant:" << result;

        //Kullanıcıdan girdi al
        QString userMessage;
        if(getUserInput(userMessage)){
            //Yeni mesajları ekleyerek devam et
            initialMessages.append(QJsonObject{{"role", "assistant"}, {"content", result}});
            initialMessages.append(QJsonObject{{"role", "user"}, {"content", userMessage}});
             chat->send(initialMessages);
        }else{
          QCoreApplication::quit();
        }

    });


    QObject::connect(chat, &ChatCompletion::error, [&](const QString &error) {
        qDebug() << "Chat Error:" << error;
    });


     // Embedding Example
    QObject::connect(embedding, &Embedding::embeddingReady, [&](const QJsonArray& embeddingResult){
        qDebug() << "Embedding:" << embeddingResult;
    });

    QObject::connect(embedding, &Embedding::error, [&](const QString &error) {
        qDebug() << "Embedding Error:" << error;
    });

    embedding->getEmbedding("Hello world");


    // Image Generation (DALL-E) Example
    QObject::connect(imageGen, &ImageGeneration::imageReady, [&](const QList<QUrl>& imageUrls){
        qDebug() << "Generated Image URLs:";
        for(const QUrl& url : imageUrls){
            qDebug() << url.toString();
        }
    });

      QObject::connect(imageGen, &ImageGeneration::error, [&](const QString &error) {
        qDebug() << "Image Generation Error:" << error;
    });

    imageGen->generateImage("A cute cat wearing a hat", "1024x1024", 1, "hd", "natural");


    return a.exec();
}
