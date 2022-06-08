#include "Hex.h"

ostream& operator<<(ostream& out, const Board& b)
{
    switch (static_cast<short> (b))
    {
        case 0: out << 'F'; break;
        case 1: out << 'B'; break;
        case 2: out << 'R'; break;
    }

    return out;
}


Hex::Hex(short num) : Graph(num*num), matrixSize(num)
{
    //Set the vector moves with a size of total nodes and a value of Board::free
    Hex::moves.assign(totalNodes, Board::FREE);

    //make hex neighbours a matrix with a size of totalNodes
    Hex::hexNeighbours.resize(totalNodes);

    //Set the neighbouring nodes of each node to true of the hex game(bidirectional graph)
    for (short i = 0; i < totalNodes; ++i)
    {
        Hex::hexNeighbours[i].assign(totalNodes, false);

        //convert the nodes to the point equivalence x = totalNodes/matrixSize and y = totalNodes%matrixSize
        point coords = convertToPoint(matrixSize, i);

        if (get<1>(coords)-1 >= 0)
            Hex::hexNeighbours[i][i-1] = true;

        if (get<1>(coords)+1 < matrixSize)
             Hex::hexNeighbours[i][i+1] = true;

        if (get<0>(coords)+1 < matrixSize)
        {
            Hex::hexNeighbours[i][i+matrixSize] = true;
            if (get<1>(coords)-1 >= 0)
                Hex::hexNeighbours[i][i+matrixSize-1] = true;
        }

        if (get<0>(coords)-1 >= 0)
        {
            Hex::hexNeighbours[i][i-matrixSize] = true;
            if (get<1>(coords)+1 < matrixSize)
                Hex::hexNeighbours[i][i-matrixSize+1] = true;
        }

    }

}

void Hex::drawBoard()
{
    //get all the moves played on the board
    map<short, Board> movePlayer;
    map<short, Board> moveComputer;

    for (const auto& p : player)
        movePlayer[convertToNode(matrixSize, p)] = playerMode;

    for (const auto& c : computer)
        moveComputer[convertToNode(matrixSize, c)] = computerMode;

    //merge vectors and sort in ascending order
    auto moves = movePlayer;
    moves.insert(moveComputer.begin(), moveComputer.end());
    print(moves.begin(), moves.end());

}

template <class Iter>
void Hex::print(Iter cursor, Iter movesEnd)
{
    short countSpace{1};


    //draw a board for the hex game
    for (int i = 0; i < totalNodes; ++i)
    {
        if ((i != 0) && (i%matrixSize == 0))
        {
            cout << endl;

            for (int k = 0; k < countSpace; ++k)
            {
                cout << " ";
            }

            for (int j = 0; j < matrixSize; ++j)
            {
                if (j == matrixSize - 1)
                    cout << "\\";
                else
                     cout << "\\ / ";

            }
            cout << endl;
            ++countSpace;

            for (int k = 0; k < countSpace; ++k)
            {
                cout << " ";
            }
            ++countSpace;
        }
        if (cursor != movesEnd && i == cursor -> first)
        {
            cout << cursor -> second << " ";
            ++ cursor;
        }

        else
            cout << ". ";

        if (i%matrixSize != matrixSize-1) cout << "- ";
    }

    cout << endl;
}

Hex::~Hex()
{
    //dtor
}
