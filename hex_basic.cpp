/*
    Interactive game of Hex
    User plays against "robot" and can choose to be player 1 or 2
    User enters desired move (i, j) into terminal
    One player wins

    Author: Deckard Mehdy
    Date: Oct. 19th, 2021

    Class Directory:
        @Node       => Creates a node object for the Graph class
        @Graph      => Creates an MxN graph object that represents the hex game
        @TreeNode   => Creates a node object for the Tree class
        @Tree       => Creates a tree object that is used to predict "robot" next move:
                        - Utilizes Monte Carlo simulation and Min-Max algorithm to select next move
*/

#include <iostream>     // cout, cin
#include <vector>       // vector
#include <assert.h>     // assert
#include <queue>        // queue
#include <algorithm>    // shuffle

using namespace std;

class Node {
    /*  Params:
            @coordI         => I (row) coordinate for node
            @coordJ         => J (column) coordinate for node
            @neighbors      => vector to store pointers to neighboring nodes
            @val            => value of node:
                                1. 0    = blank
                                2. 1    = player 1
                                3. 2    = player 2
    */

    public:
        const int coordI, coordJ;
        vector<Node*> neighbors;
        int val;

        Node(int i, int j) : coordI(i) , coordJ(j), val(0){} 

        void setVal(int newVal){ val = newVal; }

        friend ostream& operator<<(ostream& os, Node* myNode);
}; // end of Node class

ostream& operator<<(ostream& os, Node* myNode){
    string nodeValChar;
    if (myNode->val == 0){
        nodeValChar = ".";
    } else if ( myNode->val == 1){
        nodeValChar = "X";
    } else {
        nodeValChar = "O";
    }
    os << nodeValChar;
    return os;
}

class Graph {
    /*  Params:
            @rows       => number of rows 
            @cols       => number of columns
            @myGraph    => graph of nodes
            @winner     => winner of the game
                            - Winner = 0 => Inconclusive
                            - Winner = 1 => Player 1
                            - Winner = 2 => Player 2
    */

    public:

    const int rows, cols;
    int winner;
    vector< vector<Node*> > myGraph;

        Graph(int r, int c) : rows(r), cols(c), winner(0) { initGraph(); }

        void printGraph(){
            cout << endl;
            for (int i = 0; i < rows; i++){
                // Print line for data
                for (int k = 0; k < i; k++){
                    if (i == 0){ cout << " "; } else { cout << "  "; }
                }
                for (int j = 0; j < cols; j++){
                    cout << myGraph[i][j];
                    if (j != (cols - 1)){ cout << " - "; }
                }
                cout << endl;

                // Print line for edges
                for (int k = 0; k < i; k++){
                    if (i == 0){ cout << " "; } else { cout << "  "; }
                }
                if (i != (rows - 1)) {
                    for (int j = 0; j < cols; j++){
                        cout << " \\";
                        if (j != (cols - 1)){ cout << " /"; }
                    }
                }
                cout << endl; 
            }
        }

        bool isLegal(int& row, int& col){
            return (row >= 0) && (col >= 0) && (row < rows) && (col < cols) && (myGraph[row][col]->val == 0);
        }

        // Perform breadth-first search to determine if player won the game
        bool checkForWinner(const int& player){
            vector< vector<bool> > visited( rows, vector<bool>( cols, false ) ); 
            queue<Node*> myQueue;
            // Start from nodes on row-0 or col-0
            int rowsOrCols = player == 1 ? rows : cols;
            for (int x = 0; x < rowsOrCols ; x++){
                if ( player == 1 ){
                    if( myGraph[x][0]->val == player ){
                        myQueue.push( myGraph[x][0] );
                    }
                } else {
                    if( myGraph[0][x]->val == player ){
                        myQueue.push( myGraph[0][x] );
                    }
                }
            }
            // Determine if a player has made a successful connection
            while ( !myQueue.empty() ){
                Node* currNode = myQueue.front();
                myQueue.pop();
                visited[currNode->coordI][currNode->coordJ] = true;
                if ( exploreNeighbors(player, currNode, myQueue, visited) ) { return true; }
            }
            return false;
        }

    private:

        void initGraph(){
            for (int i = 0; i < rows; i++){
                vector<Node*> row;
                for (int j = 0; j < cols; j++){
                    Node* newNode = new Node(i, j);
                    row.push_back( newNode );
                }
                myGraph.push_back(row);
            }
            assignNeighbors();
        }

        void assignNeighbors(){
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    int totalNeighbors = 0;
                    Node* currNode = myGraph[i][j];
                    if ( j < (cols - 1) ){ currNode->neighbors.push_back( myGraph[i][j + 1] ); }                    // right
                    if ( j > 0 ){ currNode->neighbors.push_back( myGraph[i][j - 1] ); }                             // left
                    if ( i > 0 ){ currNode->neighbors.push_back( myGraph[i - 1][j] ); }                             // upper left
                    if ( i > 0 && j < (cols - 1) ){ currNode->neighbors.push_back( myGraph[i - 1][j + 1] ); }       // upper right
                    if ( i < (rows - 1) && j > 0 ){ currNode->neighbors.push_back( myGraph[i + 1][j - 1] ); }       // lower left
                    if ( i < (rows - 1) ){ currNode->neighbors.push_back( myGraph[i + 1][j] ); }                    // lower right
                }
            }
        }

        bool exploreNeighbors(const int& player, Node* &currNode, queue<Node*> &myQueue, vector< vector<bool> > &visited){
            vector<Node*> neighbors = currNode->neighbors;
            for( int i = 0; i < neighbors.size() ; i++ ){
                Node* neighbor = neighbors[i];
                if( !visited[neighbor->coordI][neighbor->coordJ] && neighbor->val == player ){
                    if ( player == 1 && neighbor->coordJ == (cols - 1) ){     // Player 1 wins
                        winner = 1;
                        return true;
                    }
                    if ( player == 2 && neighbor->coordI == (rows - 1) ){     // Player 2 wins
                        winner = 2;
                        return true;
                    }
                    myQueue.push( neighbor );
                }
            }
            return false;
        }
}; // end of Graph class

class TreeNode {
    /*  Params:
            @children       => 2D vector containing children of tree node
            @parent         => parent of tree node
            @p1wins         => number of player 1 wins
            @p2wins         => number of player 2 wins
            @row            => row number of node in Graph
            @col            => col number of node in Graph
    */

   public:

    vector<TreeNode*> children;
    TreeNode* parent;
    double p1wins, p2wins;
    int row, col;

    TreeNode(): parent(nullptr), p1wins(0.0), p2wins(0.0){}

    void addChild( TreeNode* &child ){ children.push_back( child ); }

    void addParent( TreeNode* &parent ){ this->parent = parent; }

    void addResult( int& winner ){ if ( winner == 1 ){ p1wins++; } else { p2wins++; } }

    void addCoords( int& i, int& j ){ row = i; col = j; }

    double getProb( const int& player ){
        int num = player == 1 ? p1wins : p2wins; 
        return num / ( p1wins + p2wins );
    }

}; // End of TreeNode class

class Tree {
    /*  Params:
            @root       => root node of tree
            @depth      => depth of tree
            @thisGraph  => Graph object linked to Tree object
            @MAX_DEPTH  => maximum depth of tree
            @MAX_TRIALS => maximum number of trials to run (Monte Carlo)
            @nextMove   => pointer to next move for graph object
    */

    public:

        TreeNode* root;

        Tree( Graph* &g ) : MAX_TRIALS(10), MAX_DEPTH(2) {
            thisGraph = g;
            depth = 0;
            for (int i = 0; i < g->rows; i++){
                for (int j = 0; j < g->cols; j++){
                    if ( g->myGraph[i][j]->val == 0){ depth++; }
                }
                if ( depth >= MAX_DEPTH ){ 
                    depth = MAX_DEPTH; 
                    break; 
                } 
            }
            initTree();
        }

        vector<int> getNextMove( int player ){
            monteCarlo( root, switchPlayer(player) );
            minMax( root, true, player, 0);
            vector<int> coords;
            coords.push_back(nextMove->row);
            coords.push_back(nextMove->col);
            return coords;
        }

    private:

        int depth;
        const int MAX_DEPTH, MAX_TRIALS;
        Graph* thisGraph;
        TreeNode* nextMove;

        double minMax( TreeNode* node, bool maximizingPlayer, int player, int depth ){
            if (depth == MAX_DEPTH){
                return node->getProb( player );
            } 
            if (maximizingPlayer){
                double val = -1;
                for(int i = 0; i < node->children.size(); i++){
                    double newVal = minMax( node->children[i], false, player, depth + 1 );
                    if ( newVal > val ){
                        val = newVal;
                        if ( depth == 0 ){ nextMove = node->children[i]; } //root node
                    }
                }
                return val;
            } else {
                double val = 2;
                for(int i = 0; i < node->children.size(); i++){
                    val = min( val, minMax( node->children[i], true, player, depth + 1 ));
                }
                return val;
            }
        }

        void monteCarlo( TreeNode* node, int player ){
            if ( node != root ){ thisGraph -> myGraph[node->row][node->col] -> setVal(player); }  // make move
            if ( node->children.size() == 0){   
                // Leaf node: start trial simulation 
                runTrials( node , switchPlayer(player) );
            } else {                            
                // Internal node: continue traversing tree
                for (int i = 0; i < node->children.size(); i++){
                    monteCarlo( node->children[i], switchPlayer( player ) );
                }
            }
            if ( node != root ){ thisGraph -> myGraph[node->row][node->col] -> setVal(0); }       // release move
            return;
        }

        void runTrials ( TreeNode* node, int player ){
            vector<vector<int> > moves = nextMoves();
            for ( int i = 0; i < MAX_TRIALS; i++ ){
                random_shuffle( moves.begin(), moves.end());
                int nextPlayer = player;
                for ( int j = 0; j < moves.size(); j++ ){
                    int r = moves[ j ][0]; // row
                    int c = moves[ j ][1]; // col
                    thisGraph -> myGraph[r][c] -> setVal(nextPlayer);
                    nextPlayer = switchPlayer(nextPlayer);
                }
                int winner = thisGraph -> checkForWinner(1) ? 1 : 2;
                if (winner == 1){ node->p1wins++; } else { node->p2wins++; }
            }
            resetGraph( moves );
        }

        void resetGraph( vector<vector<int> > &moves){
            for (int i = 0; i < moves.size(); i++){
                thisGraph -> myGraph[ moves[i][0] ][ moves[i][1] ] -> setVal(0);
            }
            thisGraph->winner = 0;
        }

        int switchPlayer( int player ){ return player == 1 ? 2 : 1; }

        void initTree(){
            this->root = new TreeNode();
            createTree( this->root, depth);
        }

        void createTree( TreeNode* node, int currDepth){
            if (currDepth <= 0){ return; }
            if ( node != this->root ){ thisGraph->myGraph[node->row][node->col]->setVal(1); }  // make move
            vector< vector<int> > children = nextMoves();
            for (int i = 0; i < children.size(); i++){
                TreeNode* child = new TreeNode();
                child->addCoords( children[i][0], children[i][1] );
                child->addParent( node );
                node->addChild( child );
                createTree( child , currDepth - 1 );
            }
            if ( node != this->root ){ thisGraph->myGraph[node->row][node->col]->setVal(0); }  // release move
        }

        // Return moves possible on graph
        vector<vector<int> > nextMoves(){
            vector<vector<int> > moves;
            for (int i = 0; i < thisGraph->rows ; i++){
                for (int j = 0; j < thisGraph->cols ; j++){
                    if ( thisGraph->myGraph[i][j]->val == 0 ){
                        vector<int> move;
                        move.push_back(i);
                        move.push_back(j); 
                        moves.push_back(move);
                    }
                }
            }
            return moves;
        }

}; // End of Graph class

void getPlayerNextMove(Graph*& g, int& player){
    int row = -1, col = -1;
    while ( !g->isLegal(row, col) ){
        cout << "Please input the row in which you would like to place your next move: ";
        cin >> row;
        cout << "Please input the column in which you would like to place your next move: ";
        cin >> col;
    }
    g->myGraph[row][col]->setVal(player);
    cout << "You have played your move at [" << row <<  ", " << col << "]" << endl;
}

void getRobotNextMove(Graph*& g, const int& player){
    cout << endl << "Robot is thinking of its next move..." << endl;
    Tree* myTree = new Tree( g );
    vector<int> nextMove = myTree->getNextMove( player );
    g->myGraph[ nextMove[0] ][ nextMove[1] ]->setVal(player);
    cout << "Robot has played a move at [" << nextMove[0] <<  ", " << nextMove[1] << "]" << endl;
    free( myTree );
}

int main(){
    int userRows, userCols, userPlayer;

    cout << endl << "Welcome to the game of Hex!" << endl;
    cout << "How many rows would you like? It must be greater than one. ";
    cin >> userRows;
    assert(userRows > 1);

    cout << "How many columns would you like? It must be greater than one. ";
    cin >> userCols;
    assert(userCols > 1);

    cout << "Would you like to be player 1 or 2? ";
    cin >> userPlayer;
    assert(userPlayer == 1 || userPlayer == 2);

    if (userPlayer == 1){
        cout << endl << "Your objective is to connect a tile from column 0 to a tile from column " << (userCols - 1) << endl;
    } else {
        cout << endl << "Your objective is to connect a tile from row 0 to a tile from row " << (userRows - 1) << endl;
    }

    // Loop until winner is found
    Graph* myGraph = new Graph(userRows, userCols);
    int currPlayer = 1;
    while( myGraph->winner == 0 ){
        if ( currPlayer == userPlayer ){
            myGraph->printGraph();
            getPlayerNextMove(myGraph, userPlayer);
        } else {
            getRobotNextMove(myGraph, currPlayer);
        } 
        if  ( myGraph->checkForWinner(1) ||  myGraph->checkForWinner(2) ){ break; }
        currPlayer = currPlayer == 1 ? 2 : 1 ; // switch
    }
    myGraph->printGraph();
    cout << "Winner is " << myGraph->winner << endl;

    free(myGraph);
    return 0;
}
