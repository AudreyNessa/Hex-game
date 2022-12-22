#include <iostream>
#include "Hex.h"

using namespace std;

int main()
{
    Hex newBoard(8);   //create an 8-board hex game
    Board winner{Board::FREE}; //stores the winner of a game. Stores the value FREE if no one has won
    short player{}; //human player
    short computer{};
    point playerMove;
    point computerMove;

    //Ask the player what colour he wants and set the modes
    do
    {
        cout << "Type 1 for black or 2 for red" << endl;
        cin >> player;

        //if the type entered is the wrong type clear it and
        //set player to an invalid number to continue the loop
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(256, '\n'); //ignore the line change
            player = 3;
        }


    }while(player < 1 || player > 2);

    computer = (player == 1) ? 2 : 1;

    newBoard.setComputerMode(static_cast<Board>(computer));
    newBoard.setPlayerMode(static_cast<Board>(player));

    //let the computer play first if its black else let the human go first
    if (computer == 1)
    {
        computerMove = newBoard.computerPlay();
        newBoard.drawBoard();
    }

    while (winner == Board::FREE)
    {
        //get who's turn it is to play
        do
        {
            cout << "Please input a valid move in this order \"x y\" with a space in between" << endl;
            cin >> get<0>(playerMove) >> get<1>(playerMove);

            //if the type entered is wrong clear it and set the
            //player move to an invalid move to continue the loop
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(256, '\n'); //ignore the line change
                playerMove = computerMove;
            }

        }while (!newBoard.validMove(playerMove));

        //add the move to list of player moves
        newBoard.addPlayerMove(playerMove);

        computerMove = newBoard.computerPlay();
        newBoard.drawBoard();
        winner = newBoard.whoWon();

    }

    //print out who won
    if (winner == static_cast<Board>(player))
        cout << "You Won!" << endl;
    else
        cout << "You Lost! " << endl;

    return 0;
}
