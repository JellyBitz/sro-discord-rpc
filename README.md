# sro-discord-rpc
Attract more players by using Discord Rich Presence on Silkroad Online!

### Features
- Game status
- Charname, level, and guildname if exists
- Elapsed time after login
- Job mode status
- Region name and his related image (is not shown if is in job mode)
- Updates level, and regions almost in realtime!
- It won't affect client performance

### Preview
<img title="User Preview" src="https://i.imgur.com/Ii2G3JQ.png">

### Starter Guide:
1. Create an application on [discord developer portal](https://discord.com/developers/applications)
2. Fill some data there but take note about your app **CLIENT ID**
3. Then go to _Rich Presence > Art Assets_ and upload all the images from folder `sro-discord-rpc/Resources/assets`
4. Open your Visual Studio 20XX and set your **CLIENT ID** into `AppManager.cpp`
5. Build / compile it on **RELEASE** mode, then copy `sro-discord-rpc.dll` and `discord_game_sdk.dll` to your silkroad folder
6. Inject `sro-discord-rpc.dll` into your client and it's done!

Note:
- Used **VS2019** & **STUD_PE** for DLL injection as personal preference.
- Compiled on v80 toolset to avoid installing extra libraries for the ending user.

### How to install v80 toolset?

- Please, follow the [SRO_DevKit Guide](https://florian0.gitlab.io/sro_devkit/build-environment/visual-studio/2005/) for installing VS2005 with only the required fields for it.

---
> ### Do you like this project ? 
> ### Support me! [Buy me a coffee <img src="https://twemoji.maxcdn.com/2/72x72/2615.png" width="18" height="18">](https://www.buymeacoffee.com/JellyBitz "Coffee <3")
> 
> ### Created with [<img title="Yes, Code!" src="https://twemoji.maxcdn.com/2/72x72/1f499.png" width="18" height="18">](#)

---
> ##### Hey Thanks!
> - [florian0](https://gitlab.com/florian0/) for his hooks and asm stuffs!
> - [#HB](https://www.elitepvpers.com/forum/members/7260849--hb.html) for his past works and sources