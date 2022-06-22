#ifndef HEX_H
#define HEX_H
#include <map>
#include <tuple>
#include "Graph.h"

typedef tuple<short, short> point;
enum class Board{FREE, BLACK, RED};
ostream& operator<<(ostream&, const Board&);

class Hex : public Graph
{
    public:
        //Constructor
        //creates a num*num hex game
        Hex(short num);

        //setters

        //sets the mode for the player
        inline void setPlayerMode(Board b){playerMode = b;}

        //sets the mode for the computer
        inline void setComputerMode(Board b){computerMode = b;}

        //adds a move for the player
        void addPlayerMove(point);

        //getters

        //returns the mode of the user (if it's player1 or player2)
        inline Board getPlayerMode() const {return playerMode;}

        //returns the mode for the computer (if it's player1 or player2)
        inline Board getComputerMode() const {return computerMode;}

        //methods

        //converts

        //draws a hex board on the screen
        void drawBoard();

        //checks if a player makes a valid move and returns true if valid
        bool validMove(point);

        //returns the player(black or red) that won the match and returns free if no one has one
        Board whoWon();

        //makes the AI play a valid move on the board
        void computerPlay();

        virtual ~Hex();

    protected:
        short matrixSize; // size of the matrix
        Board playerMode; // the mode of the players(red or black)
        Board computerMode; //the mode of the computer(red or black)
        vector<vector<bool>>hexNeighbours; //the neighbours of a node on the hex board
        vector<vector<bool>>hexHasEdge;
        vector<point> player; // moves played by the player
        vector<point> computer; //moves played by the computer
        vector<Board> moves; //moves already played

        //functions

        //converts a point to its equivalent node value
        inline short convertToNode(short num, point pnt){return get<0>(pnt) * num + get<1>(pnt);}

        //converts a node to its equivalent point value
        inline point convertToPoint(short num, short node){point newPoint(node/num, node%num); return newPoint;}

         //displays the hex board and moves played by the players to the screen
        template<class Iter>
        void print(Iter, Iter);

        //checks if a player made a path from the north to south or east to west on the hex board
        bool checkLinked(const vector<short>&, const vector<short>&);


    private:
};

#endif // HEX_H
