#include "MainWindow.h"
#include <QApplication>
#include <QSettings>

static QString getSetting(QSettings & settings, QString key, QString def = QString())
{
    auto setting = settings.value(key, "").value<QString>();
    if(!setting.length())
        settings.setValue(key, def);
    return setting;
}

static ChatConfig getChatConfig(QSettings & settings, const QString & chat, const QString & server)
{
    ChatConfig config;
    config.server = getSetting(settings, chat + "/server", server);
    config.user = getSetting(settings, chat + "/user");
    config.pass = getSetting(settings, chat + "/pass");
    config.channel = getSetting(settings, chat + "/channel");
    config.welcome = getSetting(settings, chat + "/welcome", QString("Hello, I will be syncing between Livecoding/Twitch!"));
    return config;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    MainWindow w(getChatConfig(settings, "Livecoding", "livecoding.tv"), getChatConfig(settings, "Twitch", "irc.twitch.tv"));
    w.show();
    return a.exec();
}
