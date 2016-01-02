#ifndef MAINWINDOWCLOSETHREAD_H
#define MAINWINDOWCLOSETHREAD_H

#include <QThread>
#include "ChatBot.h"

class MainWindowCloseThread : public QThread
{
    Q_OBJECT
public:
    void addBot(ChatBot* bot);

signals:
    void canClose();

private:
    QList<ChatBot*> mBots;

    void run();
};

#endif // MAINWINDOWCLOSETHREAD_H
