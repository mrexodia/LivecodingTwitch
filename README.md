# LivecodingTwitch

Bot to synchronize [Livecoding](https://livecoding.tv) and [Twitch](https://twitch.tv) stream chats. This application is designed for streamers who want to stream on both services, but only use one of the chat clients. It is recommended to create separate accounts that run the chatbot.

Feature requests/bug reports go [here](https://github.com/mrexodia/LivecodingTwitch/issues).

# Compiling

*You can skip this step if you downloaded a binary release.*

1. Clone the project (make sure to [clone the submodules](http://stackoverflow.com/questions/3796927/how-to-git-clone-including-submodules));
1. Install [Qt](http://www.qt.io/download);
2. Open and compile `libcommuni/libcommuni.pro` with Qt creator in a directory (for example `build-x64-debug`);
3. Open and compile `qxmpp/qxmpp.pro` in the same directory as `libcommuni`;
4. Open and compile `LivecodingTwitch/LivecodingTwitch.pro` in the same directory as `qxmpp` and `libcommuni`;
5. Have fun!

*Notice*: On Ubuntu you need to install `libgl1-mesa-dev` and `g++`. You may also need to copy the shared libraries to the appropriate directories.

# Configuration

1. Open `LivecodingTwitch`;
2. Close the application (you need to change your settings first);
3. Open the created `settings.ini` file and set your configuration:

```
[Twitch]
server=irc.twitch.tv
user=twitchUsername
pass=twitchOauth
channel=#twitchChannel
welcome="Hello, I will be syncing between Livecoding/Twitch today!"

[Livecoding]
server=livecoding.tv
user=livecodingUsername
pass=livecodingPassword
channel=livecodingChannel@chat.livecoding.tv
welcome="Hello, I will be syncing between Livecoding/Twitch today!"
``` 

1. Replace `twitchUsername` with your Twitch username;
2. Replace `twitchOauth` with the OAuth token from [here](http://www.twitchapps.com/tmi);
3. Replace `twitchChannel` with your Twitch channel you want to sync with (`http://twitch.tv/twitchChannel`). **Make sure** to leave the `#` in front of `twitchChannel`!
4.  Replace `livecodingUsername` with your Livecoding username;
5.  Replace `livecodingPassword` with your Livecoding password. See [here](http://blog.livecoding.tv/2015/11/21/how-to-chat-onal-xmpp-client) how to set your password if you logged in with a social account;
6.  Replace `livecodingChannel` with the channel you want to sync with (`http://livecoding.tv/livecodingChannel`). **Make sure** to leave the `@chat.livecoding.tv` part intact!
7.  You should now be able to use the application, enjoy!