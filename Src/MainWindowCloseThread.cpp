#include "MainWindowCloseThread.h"

void MainWindowCloseThread::addBot(ChatBot* bot)
{
    if(!bot)
        return;
    bot->Disconnect();
    mBots.append(bot);
}

void MainWindowCloseThread::run()
{
    while(true)
    {
        int disconnectedCount = 0;
        for(auto bot : mBots)
        {
            if(!bot->IsConnected())
                disconnectedCount++;
        }
        if(disconnectedCount == mBots.length())
            break;
        QThread::msleep(50);
    }
    emit canClose();
}
