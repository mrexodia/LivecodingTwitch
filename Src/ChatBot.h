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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtextdocument.h>
#endif //QT_VERSION

inline static QString EscapeHtml(const QString & string)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    return string.toHtmlEscaped();
#else
    return Qt::escape(string);
#endif //QT_VERSION
}

class ChatBot : public QObject
{
    Q_OBJECT
public:
    explicit ChatBot(const ChatConfig & config, QObject* parent = 0);
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual bool IsConnected() = 0;
    virtual void SendMessage(const QString & message) = 0;
    virtual QString Name() = 0;

signals:
    void logMessage(const QString & message);
    void connected();
    void disconnected();
    void messageReceived(const QString & from, const QString & message);

protected:
    ChatConfig mConfig;

    void addLogMessage(const QString & message);
};

#endif // CHATBOT_H
