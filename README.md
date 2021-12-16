# Hex

**Hex** is an interactive board game where a user plays against a “robot” on an MxN Hex board. A user can choose to be Player 1 or 2, and can choose between two modes of difficulty: normal and hard. I originally got the idea for this game from one of my C/C++ course projects, of which I’ve included in this project’s directory.

You can play the game by clicking the link below. Enjoy!
- [Link to game](https://deckardmehdy.github.io/Hex/)

---
# How To Play

Each player is assigned a pair of opposite sides of the board (as shown below). Player 1 is assigned the left and right (green); Player 2 is assigned the top and bottom (purple). Each player must try to connect their respective sides of the board by placing hexagons with their unique color.

![image](https://github.com/deckardmehdy/deckardmehdy.github.io/blob/master/Hex/images/rule_1.jpg) 


Once placed, the hexagons cannot be moved or erased. A player wins by successfully connecting their sides together through a chain of adjacent hexagons. Examples of each player winning are shown below.

![image](https://github.com/deckardmehdy/deckardmehdy.github.io/blob/master/Hex/images/player_one_wins.jpg) 

*Example of Player 1 winning (in blue)*

![image](https://github.com/deckardmehdy/deckardmehdy.github.io/blob/master/Hex/images/player_two_wins.jpg)

*Example of Player 2 winning (in blue)*

---
# The “Robot”

For the “robot”, I combined two algorithms: a Min-Max Tree (with Alpha-Beta pruning) and Monte Carlo simulation.

**1. Min-Max Tree**
---------------

Firstly, a Min-Max Tree, in essence, is a multi-level tree that is able to look multiple moves ahead. The purpose of this tree is to maximize the chances of the player winning while at the same time minimizing the chances of the opponent winning, hence the name Min-Max. You can read more on Min-Max Trees here ([link](https://en.wikipedia.org/wiki/Minimax)). 

Due to the large number of leaf nodes, Alpha-Beta pruning was incorporated into the tree traversal, which led to a significant reduction of leaf nodes being visited (-80%). You can read more on Alpha-Beta pruning here ([link](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning)). .

**2. Monte-Carlo Simulation**
---------------

At each leaf node, Monte Carlo simulation was executed. This type of simulation utilizes the Law of Large Numbers. This law states that an integral described by the expected value (i.e. the chance of a player winning) may be described by taking the mean of independent samples of a variable. So, in order to get an accurate representation of A player’s chance of winning, a sufficient number of random trials need to be run and averaged.

A random trial is conducted as follows:
- Moves available are placed in a 1-D matrix
- For each trial:
	- Randomly suffle matrix
	- For each move:
		- Play move on board for player
		- Switch player
	- See who won the game and store the result
	- Clear the board of moves made
- Return probability of winning -> (number of wins) / (total number of trials)


In order to keep the runtime to a minimum, 100 random trials were conducted at each leaf node. 

**3. Difficulty**
---------------

There are two modes of difficulty for this game: normal and hard. These two modes differ by the **depth** of their respective Min-Max Trees. The normal difficulty’s tree has a depth of **two**, which means that it can see two moves ahead (i.e. best move for player -> worst move for opponent). The hard difficulty’s tree has a depth of **three**, which means that it can see three moves ahead (i.e. best move for player -> worst move for opponent -> best move for player).

I originally had three modes of difficulty, with an evil difficulty that could see four moves ahead. However, due to its long runtime (~2 minutes), I decided to remove it. 
