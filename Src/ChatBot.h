#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>

struct ChatConfig
{
    QString server;
    QString user;
    QString pass;
    QString channel;
    QString welcome;
};

class ChatBot : public QObject
{
    Q_OBJECT
public:
    explicit ChatBot(QObject* parent = 0);
    virtual void Connect() = 0;
    virtual void SendMessage(const QString & message) = 0;

signals:
    void addLogMessage(const QString & message);
    void connected();
    void disconnected();
    void messageReceived(const QString & from, const QString & message);
};

#endif // CHATBOT_H
