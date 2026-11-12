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

Game is described in more detail in [GAME-ITPW-UW.md](./GAME-ITPW-UW.md). This is a stale file (copied directly once).
It's source is in git repository: [concrateMixers-bot-fight-arena](https://github.com/neurodine-com/concrateMixers-bot-fight-arena)
which is repos that tries to resurect the game "Betorniarki" and make it deploy to Android/iOS threw out of Android Play store and iOS store. - This is the idea... hard to implement.

## The Game state - memory problem.  

The Game board -  the playground grid is not available threw a API nor any method. The Arbiter process was raisoing two programs on connecting them on it's own STDIN and thus switching from one to another and  visualizing the game on the Arbiter UI.

The BOT aka "figting program" was suposed to memorize the gameboard state of each field so that it knows where to move andwhere are the fileds that allow the move to happen (read the (GAME-ITPW-UW.md) for details).

Nowodays idea still requires the BOT program to have the momery of the grid where the oponent concrate-Mixer is placed and where it spilled it's concrate in it's move.  
This is a major problem as the programs ware EXE files (executables compiled usually from C/C++ code) that is likely NOT to hapen nowodays. The problem is if the program is placed in the docker container then ... it is ok, but still it is sandboxed there but how muny containers will be have to be spawned - and WHERE - so that 10 simultanious players can play on their phones or on web page  designed to it. Core problems apper:

How to allow people to rite it's own basic code and connect it into the game as a BOT fighting other people bots?
Answer seems be straight-forward - git hub repositories -> this would require a action to build on some special TAG like, for example:  
**`TAG`**:**concrateMixers-BotFightArena-TAG-vx.x.x**

## Having the git repo from which BOT fighting programs will be spawned - how to have it sandbox when it is a EXE?
 
GitHub action with compilation for linux OS having it's source on a linux docker image with systemd subsystem onboard.  
This would allow to run compiled programs in slices with strictly managed resources and it's own namespace - perfect sandbox.

The docker image would have to be hosted as kubernet's cluster and the binarie would have had to be uploaded to the image by a "web upload" and internal installation on this kubernet's or the Kubernets cluster would init the compilation of the program when the user request it - register/adds git hub repo to the GAME account hosted on my cloudflare site neurodine.com with subdomain assign to it like.. bfa.neurodine.com. 

The bfa.neurodine.com would host the CMS with the game board.
User would be able to register (registration starts on andoid/iOS app) and creates account on the CMS: mixers-fight-arena on bfa.neurodine.com.

There the user would be able to add it's git repository with code He would find usuable to play the "Betoniarki" GAME.

