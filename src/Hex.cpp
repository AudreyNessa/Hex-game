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

    //make hex neighbours and hexHasEdge a matrix with a size of totalNodes
    Hex::hexNeighbours.resize(totalNodes);
    Hex::hexHasEdge.resize(totalNodes);

    //Set the neighbouring nodes of each node to true of the hex game(bidirectional graph)
    for (short i = 0; i < totalNodes; ++i)
    {
        Hex::hexNeighbours[i].assign(totalNodes, false);
        Hex::hexHasEdge[i].assign(totalNodes, false);

        //convert the nodes to the point equivalence x = totalNodes/matrixSize and y = totalNodes%matrixSize
        point coords = convertToPoint(matrixSize, i);

        if (get<1>(coords)-1 >= 0)
        {
            Hex::hexNeighbours[i][i-1] = true;
            Hex::hexHasEdge[i][i-1] = true;
        }

        if (get<1>(coords)+1 < matrixSize)
        {
            Hex::hexNeighbours[i][i+1] = true;
            Hex::hexHasEdge[i][i+1] = true;
        }

        if (get<0>(coords)+1 < matrixSize)
        {
            Hex::hexNeighbours[i][i+matrixSize] = true;
            Hex::hexHasEdge[i][i+matrixSize] = true;
            if (get<1>(coords)-1 >= 0)
            {
                 Hex::hexNeighbours[i][i+matrixSize-1] = true;
                 Hex::hexHasEdge[i][i+matrixSize-1] = true;
            }
        }

        if (get<0>(coords)-1 >= 0)
        {
            Hex::hexNeighbours[i][i-matrixSize] = true;
            Hex::hexHasEdge[i][i-matrixSize] = true;
            if (get<1>(coords)+1 < matrixSize)
            {
                Hex::hexNeighbours[i][i-matrixSize+1] = true;
                Hex::hexHasEdge[i][i-matrixSize+1] = true;
            }
        }
    }

}

void Hex::addPlayerMove(point p)
{
    if (validMove(p))
    {
        //add to graph neighbours if the move is a neighbour of any of the previous moves;
        const short EMPTY = 0;
        const short PLAYER_SIZE = player.size(); //number of moves made by the player
        short node = convertToNode(matrixSize, p); //convert the point to nodes and add to moves

        //if there is the player has made more than one move check if the last move is a neighbour to any of the previous moves
        if (PLAYER_SIZE > EMPTY)
        {
            for (point m : player)
            {
                //convert the point m to its equivalent node value
                short nodePlayed = convertToNode(matrixSize, m);
                if(hexHasEdge[nodePlayed][node])
                {
                    Graph::setEdgeValue(nodePlayed, node, 1);
                }
            }
        }

        //add the move to player and moves
        player.push_back(p);
        moves[node] = playerMode;

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

bool Hex::validMove(point playerMove)
{
    bool firstValid = get<0>(playerMove) > -1 && get<0>(playerMove) < matrixSize;
    bool secondValid = get<1>(playerMove) > -1 && get<1>(playerMove) < matrixSize;

    //convert to node and check if it is a node in the board
    if (firstValid && secondValid)
    {
        short node = convertToNode(matrixSize,playerMove);
        //check if it's already used
        if (moves[node] == Board::FREE) return true;

    }
    return false;
}

Board Hex::whoWon()
{
    //add all the moves made at the edges and vertices of the board
    vector<short> playerStartMoves;
    vector<short> computerStartMoves;
    vector<short> playerEndMoves;
    vector<short> computerEndMoves;

    //check if a move was made at the edges or vertices of the board
    bool playerBeginX = false;
    bool playerBeginY = false;
    bool playerEndX = false;
    bool playerEndY = false;
    bool computerBeginX = false;
    bool computerBeginY = false;
    bool computerEndX = false;
    bool computerEndY = false;


    //get all the moves played by both players and check if there's a move played at the beginning and end
    for (const auto& p : player)
    {
        if (get<0>(p) == 0)
        {
            playerBeginX = true;
            playerStartMoves.push_back(convertToNode(matrixSize, p));
        }

        if (get<1>(p) == 0)
        {
            playerBeginY = true;
            playerStartMoves.push_back(convertToNode(matrixSize, p));
        }

        if (get<0>(p) == matrixSize - 1)
        {
            playerEndX = true;
            playerEndMoves.push_back(convertToNode(matrixSize, p));
        }

        if(get<1>(p) == matrixSize - 1)
        {
            playerEndY = true;
            playerEndMoves.push_back(convertToNode(matrixSize, p));
        }
    }

    for (const auto& c : computer)
    {
        if (get<0>(c) == 0)
        {
            computerBeginX = true;
            computerStartMoves.push_back(convertToNode(matrixSize, c));
        }

        if (get<1>(c) == 0)
        {
            computerBeginY = true;
            computerStartMoves.push_back(convertToNode(matrixSize, c));
        }

        if (get<0>(c) == matrixSize - 1)
        {
            computerEndX = true;
            computerEndMoves.push_back(convertToNode(matrixSize, c));
        }

        if (get<1>(c) == matrixSize - 1)
        {
            computerEndY = true;
            computerEndMoves.push_back(convertToNode(matrixSize, c));
        }

    }


    //check if there is a possibility of a win starting with the first player
    if (Hex::playerMode == Board::BLACK)
    {
        if ((playerBeginX && playerEndX) || (playerBeginY && playerEndY))
        {
            if (Hex::checkLinked(playerStartMoves, playerEndMoves)) return Hex::playerMode;
        }

        if ((computerBeginX && computerEndX) || (computerBeginY && computerEndY))
        {
            if (Hex::checkLinked(computerStartMoves, computerEndMoves)) return Hex::computerMode;
        }
    }
    else
    {
        if ((computerBeginX && computerEndX) || (computerBeginY && computerEndY))
        {
            if (Hex::checkLinked(computerStartMoves, computerEndMoves)) return Hex::computerMode;
        }

        if ((playerBeginX && playerEndX) || (playerBeginY && playerEndY))
        {
           if (Hex::checkLinked(playerStartMoves, playerEndMoves)) return Hex::playerMode;
        }
    }



    return Board::FREE;
}

//check if there's a path north-south or east-west for any player
bool Hex::checkLinked(const vector<short>& playerStartMoves, const vector<short>& playerEndMoves)
{
    for (short startMove : playerStartMoves)
    {
       for (short endMove : playerEndMoves)
        {
            if (shortestPath(*this, startMove, endMove) > 0)
            {
                return true;
            }
        }
    }

    return false;
}


Hex::~Hex()
{
    //dtor
}
