## Rewrite Java server example in C language ##
This laboratory work was done in WSL

## Second task ##
* Server has clients
* We have a specific clint - bot, which is responsible for censorship
* This bot has a blacklist and if one of the clients use these words from the list then:
    * the 1st time it happens, the bot will send a warning ISPEK K1 (K1 would be a client name)
    * the 2nd time it happens, the bot will send a warning ISPEK2 K1 (K1 would be a client name)
    * the 3rd time it happens, the bot will disconnect the client from the server
* If a client, that was disconnected before (fo example the same K1), connects and uses a word from the blacklist, then the bot will send a warning ISPEK3 K1 (K1 would be a client name). After this, the client K1 can never connect again.
