#ifndef QTOPENAI_H
#define QTOPENAI_H

#pragma once
#include "qtopenai_global.h"

#include <QObject>
#include <QString>

class ChatCompletion;
class Embedding;
class ImageGeneration;

class QTOPENAI_EXPORT QtOpenAI : public QObject {
    Q_OBJECT

public:
    explicit QtOpenAI(QObject *parent = nullptr);
    ~QtOpenAI();

    void setApiKey(const QString &apiKey);
    QString apiKey() const;

    ChatCompletion *chat();
    Embedding* embeddings();
    ImageGeneration* image();


private:
    QString m_apiKey;

    class Private;
    Private *d;
};

#endif // QTOPENAI_H
