- for refactoring:
  - add some function documentation to header files
  - make FNames for actor and function names constants and replace them in FindObject calls for performance
    - this turned out to be a pain in the ass because it seems FNames can only be created after unreal_init; will revisit this if performance problems arise

- see if there's a way to enforce a minimum apworld version from the client, so that people can't connect to an 0.6.0 seed with an 0.7.0 client and fuck up location ids
  - could i set a maximum client version in the apworld?

- maybe make the player obtain the game end event for fun

- have some custom death link messages check options or map name

- save files and other main menu stuff
  - consider stealing format of tunic/hacknet and placing connection menu on main menu
  - save files themselves should probably be handled by c++; they will probably be in json format
    - When the player loads a file, BP calls back to C++ to tell it to load the corresponding file
    - When the player deletes a file, BP calls back to C++ to tell it to delete the corresponding file
    - Internal state is initialized when a slot is selected
    - Update internal state whenever anything in it changes (getting an item, checking a location, etc)
  - save files should be associated with a uri, slot name, and password on first connection
    - when loading into a new save file, queue with a boolean and attempt to connect automatically once the next scene loads
    - when a connection command is input, compare the connection info to what's in the file. if it doesn't match, warn the player and make them input a special command to change the info for that save file