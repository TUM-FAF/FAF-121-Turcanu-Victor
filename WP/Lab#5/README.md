Laboratory Work Nr. 5
====================

Collaboration. Complex application.
------------

Completed Mandatory Objectives:
------------------------------

*	Create an application which has:
	-	Child windows controls
	-	GDI Animations
*	Animation should be controlled by controls

Objectives With Points
------------------------

*	Work on this project in a team of 2-4 (2 pt)
	-	Divide tasks and describe them in readme (for each task indicate who is responsible for it) (1 pt)
*	Make it useful (you may ask me to evaluate usefulness of your application idea) (0-3 pt)
*	Make the code object oriented (2pt)


The theory of workflow:
-----------------------------

We chose to work in a team of four pleople: me, A. Istratii, A. Capastru and A. Patrasko. Our application should be a battleship game with two players (can also be a bot and a player or even two bots). Here is our [repository](https://github.com/code-bits/battleship) where you can check the made work.

My task was mainly the display of the field. This should be a nice and clean one. When the user clicks on a cell, a message or an interaction should occur. There are 4 colors for each state of the cell: BLANK - dark blue, MISS - light blue, ALIVE - yellow, DEAD - brown. If you click on a BLANK cell, it becomes MISS. If you click on the MISS cell, it stays as it is. If you click on an ALIVE cell - it becomes dead. If you click on a DEAD cell - it stays as it is.

Also I've implemented the logic funtion for validating the field at the initialization. It should contain a specific number of ships and the should be placed correspondingly. Also I've implemented the first version of CheckCell() function which changes the field according to the accessed cell.
