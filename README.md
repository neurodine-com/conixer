# Title: Conixer aka BOT @ concrateMixers-Bot-Fight-Arena

## The Code

This is code orginally writen in Object C in MinGW in codeBlocks on Windows - 20 years ago - 2007.
This code worked to compile into executable that was connecting threw STD IO with java aplet Arbiter from ITPW on UW.
This code worked but had bad memory management and after fiew runs had thrown AccessViolation at some point and .. was crashing continously every time (only windows restart helped). 
On November 2026 i managed to migrate this code from stdc++ v7 to stdc++20 with AI help. 

## The Functionality

This is the BOT/fighting program on "Betorniarki" from ITPW challange from University of Warsew from 2007 year..(i think).

The program follows the Arbiter rules of moves, delivers response to stdout and memorizes the playing grid state so that it can compute things known as "the strategy" - where to move and where to spill the concrate.

## The Game

Game from 2007 from ITPW @ UW is described in detail in [concrateMixers-bot-fight-arena/GAME-ITPW-UW.md](https://github.com/neurodine-com/concrateMixers-bot-fight-arena/GAME-ITPW-UW.md). A stale file copy from the **origin** is present additionaly in [HERE](https://github.com/neurodine-com/conixer/GAME-ITPW-UW.md) but it is more like a __OFFLINE__ copy as it should not change that much.  
It describes **HOW-IT-WAS-20years-AGO** at the ITPW contest live event and how it was then made playabele for contestants - in 2007.  

The repository [concrateMixers-bot-fight-arena](https://github.com/neurodine-com/concrateMixers-bot-fight-arena) is a repo that tries to __resurect__ the game "Betorniarki" and
 ```
Make it deploy to Android/iOS threw out of Android Play store and iOS store
  - This is the idea... being tried to be implemented in "concrateMixers-bot-fight-arena".
 ```

## The Game playability problem in 2007 vs NOW  

The Game board -  the playground grid is not available threw a API nor any other method. - This is intended.  
Back then, the Arbiter process was raising two programs.  
Connecting them on it's own (Arbiter's) STDIN and continuesly  switching from one to other reading input/visualizing the game state on board/sending to other the new state.  
The Arbiter made the UI updates to display the changed game grid - visualizing new Betoniarki positions and concreate stuffed grid fields.

The BOT aka "fighting program" had to save/memorize changes on the game grid so that it known where to move, where are the fileds that allow the move to happen (it's in the GAME-ITPW-UW.md).

My (nowodays) idea is still the same - programs still should be required the BOT/program to have the momery of the grid where the oponent concrate-Mixer is placed and where it spilled it's concrate in it's move.  

This was then a major problem and is still now.  
The programs ware then EXE files (executables compiled usually from C/C++ code) and still should be like this.  
```
The development should be simple for a rookie, a developer that learns programming. I learnt a lot when attempting this contest 20 years ago.
```

The problem is with that it is a EXE that must be compiled somehow.  

Solution seems to be a sandbox made out of docker container and systemd.  
That is, if the program is placed in the docker container then ... it is one level sandbox, but still it is to much free activity space in it on it's own. The idea is that a lot of simultanious players can play at once so that for example 10 simultanious players can play on their phones or on web page  designed to it. Core problems appers:

How to allow people to write it's own basic code and connect it into the game as a BOT fighting other peoples bots?
Answer seems be straight-forward - git hub repositories -> this would require a action to build on some special TAG like, for example:  
**`TAG`**:**concrateMixers-BotFightArena-TAG-vx.x.x**

## Having the git repo from which BOT fighting programs will be spawned - how to have it sandbox when it is a EXE?
 
GitHub action with compilation for linux OS having it's source on a linux docker image with systemd subsystem onboard.  
This would allow to run compiled programs in slices with strictly managed resources and it's own namespace - perfect sandbox.

The docker image would have to be hosted as kubernet's cluster and the binary would have had to be uploaded to the image by a "web upload" and internal installation on this kubernet's or the Kubernets cluster would init the compilation of the program when the user request it - register/adds git hub repo to the GAME account hosted on my cloudflare site neurodine.com with subdomain assign to it like.. bfa.neurodine.com. 

The bfa.neurodine.com would host the CMS with the game board.
User would be able to register (registration starts on andoid/iOS app) and creates account on the CMS: mixers-fight-arena on bfa.neurodine.com. Without registration user would be allowed to play agains on automated bot - this exact on in this **conixer** repository, and use mouse/keyboard or voice to move Betoniarke on the playground grid agains the BOT.

If the user would register then, he would would be able to add it's own git repository with it's own written code. When suitable he would commit it with the TAG mentioned before and the kubernets cluster service would start doing the compilation with some unit testing of the IO and finally make a systemd  slice for the compiled program and make it available for the GAME. The slice would live as long as the game is active. When game is NOT active, systemd kills the slice.  

