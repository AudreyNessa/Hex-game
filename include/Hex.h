#ifndef HEX_H
#define HEX_H
#include <map>
#include <tuple>
#include "Graph.h"

typedef tuple<short, short> point;
enum class Board{FREE, BLACK, RED}; //black is the north-south player and red is the east-west player
ostream& operator<<(ostream&, const Board&);

class Hex : public Graph
{
    public:
        //Constructors
        //creates a num*num hex game
        Hex(short num);

        //copy constructor
        Hex(const Hex&);

        //setters

        //sets the mode for the player
        inline void setPlayerMode(Board b){playerMode = b;}

        //sets the mode for the computer
        inline void setComputerMode(Board b){computerMode = b;}

        //adds the move made by the player
        inline void addPlayerMove(point playerMove){addMove(*this, playerMode, playerMove);}

        //getters

        //returns the mode of the user (if it's player1 or player2)
        inline Board getPlayerMode() const {return playerMode;}

        //returns the mode for the computer (if it's player1 or player2)
        inline Board getComputerMode() const {return computerMode;}

        //methods

        //converts

        //draws a hex board on the screen
        void drawBoard();

        //return who's turn it is to play on the board
        inline Board whosTurn() const
        {
            short num = 0;
            //get all the moves played on the board and count them
            for (Board boardValue : moves)
                if (boardValue != Board::FREE)
                    ++num;

            //find who's turn it is to play
            cout << "computer mode " << static_cast<int>(computerMode) << " %%%" << "num " << num << endl;
            if (num % 3 == static_cast<int>(computerMode))
                return computerMode;
            else
                return playerMode;

        }

        //returns true if the board is filled and false otherwise
        inline bool isBoardFilled(const Hex& game) const
        {
            for (Board boardValue : game.moves)
                if (boardValue == Board::FREE)
                    return false;
            return true;
        }

        inline bool isBoardFilled() const {return isBoardFilled(*this);}

        //checks if a player makes a valid move and returns true if valid
        bool validMove(point);

        //returns the player(black or red) that won the match and returns free if no one has won
        Board whoWon(Hex&, const vector<point>&, const vector<point>&);
        inline Board whoWon(){return Hex::whoWon(*this, Hex::player, Hex::computer);};

        //returns the a move using the Monte Carlo simulation
        point computerPlay();

        //returns the moves not yet played on the board
        inline vector<short> getPossibleMoves(vector<Board>& boardGame) const
        {
            vector<short> possibleMoves;
            for (int i = 0; i < totalNodes; ++i)
                if (boardGame[i] == Board::FREE)
                    possibleMoves.push_back(i);

            return possibleMoves;
        }

        //copies the values a hex to another hex
        virtual void operator=(const Hex&);

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
        inline short convertToNode(short num, point pnt) const{return get<0>(pnt) * num + get<1>(pnt);}

        //converts a node to its equivalent point value
        inline point convertToPoint(short num, short node) const{point newPoint(node/num, node%num); return newPoint;}

         //displays the hex board and moves played by the players to the screen
        template<class Iter>
        void print(Iter, Iter);

        //checks if a player made a path from the north to south or east to west on the hex board
        bool checkLinked(Hex&, const vector<short>&, const vector<short>&);

        //calculates the probability a move made by a player will lead to a win and returns that value
        double monteCarlo(short);

        //adds a move for the player or computer
        void addMove(Hex&, Board, point);

    private:
};

#endif // HEX_H
