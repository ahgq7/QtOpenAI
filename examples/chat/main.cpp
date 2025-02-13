#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "qtopenai.h"
#include "models/chatcompletion.h"
#include "models/embedding.h"
#include "models/imagegeneration.h"
#include <QTimer>

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
    openai.setApiKey("OPENAI_API_KEY"); // API anahtarınızı buraya girin.

    ChatCompletion *chat = openai.chat();
    Embedding* embedding = openai.embeddings();
    ImageGeneration* imageGen = openai.image();

    int completedRequests = 0;
    int totalRequests = 3;

    auto checkAndQuit = [&]() {
        completedRequests++;
        if (completedRequests >= totalRequests) {
            QCoreApplication::quit();
        }
    };

    QTimer::singleShot(0, [&]() {

        // Chat Completion Example
        QJsonArray initialMessages;
        initialMessages.append(QJsonObject{{"role", "system"}, {"content", "You are a helpful assistant."}});
        initialMessages.append(QJsonObject{{"role", "user"}, {"content", "Merhaba, nasılsın?"}});

        //Send the first message
        chat->send(initialMessages);


        QObject::connect(chat, &ChatCompletion::ready, [&](const QString &result) {
            qDebug() << "Assistant:" << result;
            checkAndQuit();

            QString userMessage;
            if(getUserInput(userMessage)){
                initialMessages.append(QJsonObject{{"role", "assistant"}, {"content", result}});
                initialMessages.append(QJsonObject{{"role", "user"}, {"content", userMessage}});
                chat->send(initialMessages);
            }
        });


        QObject::connect(chat, &ChatCompletion::error, [&](const QString &error) {
            qDebug() << "Chat Error:" << error;
            checkAndQuit();
        });


        // Embedding Example
        QObject::connect(embedding, &Embedding::embeddingReady, [&](const QJsonArray& embeddingResult){
            qDebug() << "Embedding (first 10 elements):";
            QStringList values;
            for (int i = 0; i < std::min(10, static_cast<int>(embeddingResult.size())); ++i) {
                values << QString::number(embeddingResult[i].toDouble());
            }
            qDebug() << values.join(", ");
            checkAndQuit();
        });

        QObject::connect(embedding, &Embedding::error, [&](const QString &error) {
            qDebug() << "Embedding Error:" << error;
            checkAndQuit();
        });

        embedding->getEmbedding("Hello world");


        // Image Generation (DALL-E) Example
        QObject::connect(imageGen, &ImageGeneration::imageReady, [&](const QList<QUrl>& imageUrls){
            qDebug() << "Generated Image URLs:";
            for(int i = 0; i < imageUrls.size(); ++i) {
                qDebug() << "  Image " << (i + 1) << ":" << imageUrls[i].toString();
            }
        });

        QObject::connect(imageGen, &ImageGeneration::error, [&](const QString &error) {
            qDebug() << "Image Generation Error:" << error;
            checkAndQuit();
        });

        imageGen->generateImage("A cute cat wearing a hat", "1024x1024", 1, "hd", "natural");
    });

    return a.exec();
}
