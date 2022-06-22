#include <iostream>
#include "Hex.h"

using namespace std;

int main()
{
    Hex newBoard(8);

    newBoard.setPlayerMode(Board::BLACK);

    for (int i = 0; i < 8; ++i)
    {
        newBoard.addPlayerMove(point(i, 7));
    }

    newBoard.drawBoard();
    cout << "who won " << newBoard.whoWon();
    return 0;
}
