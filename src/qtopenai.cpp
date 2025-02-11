#include "qtopenai.h"
#include "network/networkmanager.h"
#include "models/chatcompletion.h"
#include "models/embedding.h"
#include "models/imagegeneration.h"


class QtOpenAI::Private {
public:
    Private(QtOpenAI *q) : q_ptr(q) {
        networkManager = new NetworkManager(q);
    }

    NetworkManager *networkManager;
    ChatCompletion *chatCompletion = nullptr;
    Embedding *embedding = nullptr;
    ImageGeneration *imageGeneration = nullptr;

private:
    QtOpenAI *q_ptr;
};

QtOpenAI::QtOpenAI(QObject *parent)
    : QObject(parent), d(new Private(this)) {}

QtOpenAI::~QtOpenAI() {
    delete d;
}

void QtOpenAI::setApiKey(const QString &apiKey) {
    m_apiKey = apiKey;
}

QString QtOpenAI::apiKey() const
{
    return m_apiKey;
}

ChatCompletion *QtOpenAI::chat() {
    if (!d->chatCompletion) {
        d->chatCompletion = new ChatCompletion(this, d->networkManager);
        d->chatCompletion->setApiKey(m_apiKey);
    }
    return d->chatCompletion;
}

Embedding* QtOpenAI::embeddings()
{
    if(!d->embedding) {
        d->embedding = new Embedding(this, d->networkManager);
        d->embedding->setApiKey(m_apiKey);
    }
    return d->embedding;
}

ImageGeneration* QtOpenAI::image()
{
    if(!d->imageGeneration){
        d->imageGeneration = new ImageGeneration(this, d->networkManager);
        d->imageGeneration->setApiKey(m_apiKey);
    }
    return d->imageGeneration;
}