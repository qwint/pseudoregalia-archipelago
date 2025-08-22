# pseudoregalia-archipelago
This is a mod for Pseudoregalia that works with the [multi-game multi-world randomizer Archipelago](https://archipelago.gg/). It is currently in alpha and is being actively developed. Abilities, minor upgrades, major keys, small keys, and health pieces are shuffled. Outfits are also shuffled if you are playing on current patch and the `randomize_time_trials` option is turned on.

You can get the latest release from [the releases page](https://github.com/qwint/pseudoregalia-archipelago/releases).

The logic is separated by difficulty, which you can choose in your player yaml file. The options are explained in the template.

You'll spawn in Castle with no items, but there are options to start with Dream Breaker and/or the map (if you are playing on the latest patch). If you don't have Dream Breaker yet, you can save the game by touching a save point.

You can get the PopTracker pack for Pseudoregalia [here](https://github.com/highrow623/pseudoregalia_brooty).


# Extra Features
- If you ever get stuck, entering `/spawn` into the console will save your progress and take you back to where you spawned into the game.
- If you ever find yourself without Dream Breaker after obtaining it, entering `/breaker` into the console will recall it.
- You can toggle Solar Wind once you've obtained it by pressing the top face button on controller or left ctrl on keyboard.
- Entering `/popups mute` into the console will mute the sound effects of send/receive popups.
- Entering `/popups hide` into the console will stop send/receive popups from appearing completely.
- Client settings like death link are defined in `pseudoregalia/Binaries/Win64/Mods/AP_Randomizer/settings.toml`. Releases include a template file in the same folder called `settings.tmpl.toml` which explains what is configurable.


# Mod Installation
Note: If you are installing on Steam Deck, please read [the Steam Deck installation instructions](https://github.com/qwint/pseudoregalia-archipelago/blob/main/docs/steam_deck_installation.md) instead.
1. [Go to the Releases page](https://github.com/qwint/pseudoregalia-archipelago/releases/latest) and download the latest version of `pseudoregalia-archipelago.zip`.
2. In Steam/steamapps/common, make a copy of your Pseudoregalia folder and rename it "Pseudoregalia_AP" (or whatever else you like).
   * You can add the new `pseudoregalia.exe` as a "Non-Steam Game" in Steam to access it easily from Steam and to maintain Steam Input compatibility (which may be required for some controllers)
   * If you would like to play on full gold version, downpatch using the beta code `fullgoldjump` in Steam before copying the folder. You can make an AP folder for both versions if you want to be able to play on both.
3. Extract the contents of `pseudoregalia-archipelago.zip` directly into "Pseudoregalia_AP".

![](https://i.imgur.com/SGPm9oq.jpg)

4. Create a shortcut to Pseudoregalia_AP/pseudoregalia.exe and rename it "Pseudoregalia_AP" (or whatever else you like). Place it on your desktop or in your games folder or whatever.
5. Launch the shortcut, which should open Pseudoregalia. Start a new file.
6. Once you've loaded into the game, press `enter` to open the in-game console, and enter your connect message:

`/connect ip:port slotname password`


# Generating a Seed
1. Install [Archipelago](https://archipelago.gg/tutorial/Archipelago/setup/en).
2. [Go to the Releases page](https://github.com/qwint/pseudoregalia-archipelago/releases/latest) and download the latest version of `pseudoregalia.apworld` and either double click to open the file with Archipelago, or manually place `pseudoregalia.apworld` into `Archipelago/custom_worlds` to install.
3. Place all yamls for the seed into Archipelago/Players.
   * You can get a template yaml file by clicking Generate Template Options after installing the apworld. If you just installed the apworld, you may have to restart the Archipelago Launcher before generating.
4. Run Generate from the Launcher.
5. A zip file will be added to Archipelago/output, which can [hosted on the website](https://archipelago.gg/uploads) or hosted locally by clicking Host.


# Known Issues
- The mod will not always fully load before starting the game, especially the first time. If the AP logo doesn't appear in the top right corner of the main menu, try restarting the game.
- If you're on top of an item when you first connect, it won't spawn until you reload the scene.
