# sro-discord-rpc
Attract more players by using Discord Rich Presence on Silkroad Online!

### Features
- Show game status
- Show charname, level, and guildname if exists
- Show region name and his related image
- Updates level, and region almost in realtime!

### Preview
<img src="https://i.imgur.com/oWxQ0N1.png">

### Starter Guide:
1. Create an application on [discord developer portal](https://www.elitepvpers.com/link/?https://discord.com/developers/applications)
2. Fill some data there but take note about your app **CLIENT ID**
3. Then go to _Rich Presence > Art Assets_ and upload all the images from folder `sro-discord-rpc/Resources/assets`
4. Open your Visual Studio 20XX and set your **CLIENT ID** into `AppManager.cpp`
5. Build/compile it on RELEASE mode, copy the DLL's (`sro-discord-rpc.dll` and `discord_game_sdk.dll`) to your silkroad folder
6. Inject the DLL (you could use STUD_PE or similars) and done! 

---
> ### Do you like this project ? 
> ### Support me! [Buy me a coffee <img src="https://twemoji.maxcdn.com/2/72x72/2615.png" width="18" height="18">](https://www.buymeacoffee.com/JellyBitz "Coffee <3")
> 
> ### Created with [<img title="Yes, Code!" src="https://twemoji.maxcdn.com/2/72x72/1f499.png" width="18" height="18">](#)

---
> ##### Hey Thanks!
> - florian0 for his hooks and asm stuffs!
> - #HB for his past works and sources
