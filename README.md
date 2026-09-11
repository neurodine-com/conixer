# conixer
A very veeeeery old (first code in C) buggy code from Fighting Programs Competition

Note:
This requires a Java Applet that simply is the Judge/Rule Holder and governs the game play to be conducted in fair manner by Players.

Code:
	Code was written in mingw compiller (MinGW)


Game env:
	A 2D board (a chess board but 9x9 fields weight and height)

Applet role:
	Applet creates the BOARD and reads std input from a Player (orders to move/place obsticles in form of coordinates [block addresses])
	and afterwards switches to the Player for reading it's input.
	A Player can be a program (sending it's output to feed Applet's stdin) or it can be a interactive user mous input that is translatedo to coordinates/orders to set obsticle by mouse clicks.  

Game rules:
	Block Your oponent from being able to move on the board (lock him out) / Block Your oponent from blocking You out.
	Players are allowed to move on the board in maximum distance of 3 fields BUT can not move threw obsticles.
	In each turn ZERO or ONE obsticles can be placed by a player in which case, if placed, the number of distance blocks to move by the player isreduced by 1.

Game Ends when:
	a.) one of programs crashes (access violation, code bug, program can not be run etc) - the other program is set as winner
	b.) One Player blocks the other from being able to perform any move (place obsticle)

The program i wrote:

	This is one of my first attempts to run a C program (not a pascal like my past programs) in MinGw compiler and
	I know it fallse after some time in a ACCESS VIOLATION error .. - Yeas, didn't do cleanup properly.
	It runs nice a while, but after fiew games (or randomly in first game) crashes.. and there is no end of it.
	I did not fixed it because it was nicely playing fiew games in raw and later crashing so goal achied.. kind of.

	What is suprosing on the logic of the programs?
	I manage to write (I was not aware of it at that time that it will look like this :D )
		A PROGRAM that VERY EFFECTIVELY FOLLOWS it's component and very effectively evedes the obsticles when following and additionally
		blocks very rapidly the oponent from ability to move.

	My rommate took also thic competition and at that time he was already working as a PHP developer, so he was a better codder then me, but this 
	did not allowed to win his program/code/algorithm from winning against my program. 
	He was the one that noticed that my program is doing a FOLLOW algorithm and it suprisingly did look like in effect
	that "conixer" is rather chaicing the other program. It did not followed the other program on it's exact path but 
	threw the algorithm i wrote, it was chaising the other program on a path with effect of estimating the oponnent next move by calculating
	it's most effective move possibility.
	Effect was a surprise:
		my program did not do the exact Follow (step by step in exact path of the RUNNER) but rather a CHAICE
