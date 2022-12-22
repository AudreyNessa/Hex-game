#include "Hex.h"
#include <cassert>

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
    for (short i = 0; i < Hex::totalNodes; ++i)
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

Hex::Hex(const Hex& game) : Graph(static_cast<Graph>(game))
{
    Hex::player = game.player;
    Hex::computer = game.computer;
    Hex::moves = game.moves;
    Hex::playerMode = game.playerMode;
    Hex::computerMode = game.computerMode;
    Hex::hexHasEdge = game.hexHasEdge;
    Hex::hexNeighbours = game.hexNeighbours;
    Hex::matrixSize = game.matrixSize;

}

void Hex::addMove(Hex& game, Board mode, point p)
{
    if (validMove(p))
    {
        if (mode == game.playerMode)
        {
            //add to graph neighbours if the move is a neighbour of any of the previous moves;
            const short EMPTY = 0;
            const short PLAYER_SIZE = game.player.size(); //number of moves made by the player
            short node = convertToNode(matrixSize, p); //convert the point to nodes and add to moves

            //if the player has made more than one move check if the last move is a neighbour to any of the previous moves
            if (PLAYER_SIZE > EMPTY)
            {
                for (point m : game.player)
                {
                    //convert the point m to its equivalent node value
                    short nodePlayed = convertToNode(matrixSize, m);
                    if(game.hexHasEdge[nodePlayed][node])
                    {
                        game.setEdgeValue(nodePlayed, node, 1);
                    }
                }
            }

            //add the move to player and moves
            game.player.push_back(p);
            game.moves[node] = game.playerMode;
        }
        else
        {
            //add to graph neighbours if the move is a neighbour of any of the previous moves;
            const short EMPTY = 0;
            const short COMPUTER_SIZE = game.computer.size(); //number of moves made by the computer
            short node = convertToNode(matrixSize, p); //convert the point to nodes and add to moves

            //if the computer has made more than one move check if the last move is a neighbour to any of the previous moves
            if (COMPUTER_SIZE > EMPTY)
            {
                for (point m : game.computer)
                {
                    //convert the point m to its equivalent node value
                    short nodePlayed = convertToNode(matrixSize, m);
                    if(game.hexHasEdge[nodePlayed][node])
                    {
                        game.setEdgeValue(nodePlayed, node, 1);
                    }
                }
            }

            //add the move to computer and moves
            game.computer.push_back(p);
            game.moves[node] = game.computerMode;
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

Board Hex::whoWon(Hex& game, const vector<point>& playerMoves, const vector<point>& computerMoves)
{
    //stores the moves made at the edges of the board
    vector<short> playerStartMoves;
    vector<short> computerStartMoves;
    vector<short> playerEndMoves;
    vector<short> computerEndMoves;

    //checks if a move was made at the edges or vertices of the board
    bool playerBegin = false;
    bool playerEnd = false;
    bool computerBegin = false;
    bool computerEnd = false;

    //checks if the player is a north-south player(BLACK) or an east-west player(RED) and stores values accordingly
    if (Hex::playerMode == Board::BLACK)
    {
        //get all the moves played by both players and check if there's a move played at the beginning and end (the edges of the board)
        for (const auto& p : playerMoves)
        {

            if (get<0>(p) == 0)
            {
                playerBegin = true;
                playerStartMoves.push_back(convertToNode(matrixSize, p));
            }

            else if (get<0>(p) == matrixSize - 1)
            {
                playerEnd = true;
                playerEndMoves.push_back(convertToNode(matrixSize, p));
            }
        }

        for (const auto& c : computerMoves)
        {

            if (get<1>(c) == 0)
            {
                computerBegin = true;
                computerStartMoves.push_back(convertToNode(matrixSize, c));
            }

            else if (get<1>(c) == matrixSize - 1)
            {
                computerEnd = true;
                computerEndMoves.push_back(convertToNode(matrixSize, c));
            }

        }
    }
    else
    {
        for (const auto& p : playerMoves)
        {
            if (get<1>(p) == 0)
            {
                playerBegin = true;
                playerStartMoves.push_back(convertToNode(matrixSize, p));
            }
            else if(get<1>(p) == matrixSize - 1)
            {
                playerEnd = true;
                playerEndMoves.push_back(convertToNode(matrixSize, p));
            }
        }

        for (const auto&c : computerMoves)
        {
            if (get<0>(c) == 0)
            {
                computerBegin = true;
                computerStartMoves.push_back(convertToNode(matrixSize, c));
            }

            else if (get<0>(c) == matrixSize - 1)
            {
                computerEnd = true;
                computerEndMoves.push_back(convertToNode(matrixSize, c));
            }
        }

    }


    //check if a player has won the game
    if ((playerBegin && playerEnd))
    {
        if (Hex::checkLinked(game, playerStartMoves, playerEndMoves))
            return Hex::playerMode;
    }

    if ((computerBegin && computerEnd))
    {
        if (Hex::checkLinked(game, computerStartMoves, computerEndMoves))
            return Hex::computerMode;
    }

    return Board::FREE;
}


bool Hex::checkLinked(Hex& game, const vector<short>& playerStartMoves, const vector<short>& playerEndMoves)
{
    //convert the hex game to a graph to use the shortest path function
    Graph gameGraph(static_cast<Graph>(game));

    //find if there is any link between the moves from one end to another
    for (short startMove : playerStartMoves)
    {
       for (short endMove : playerEndMoves)
        {
            if (shortestPath(gameGraph, startMove, endMove) > 0)
            {
                return true;
            }
        }
    }

    return false;
}

point Hex::computerPlay()
{
    double highestProb = 0.0; //stores the highest probability of the computer wining
    double newProb = 0.0; //stores the probability the computer winning after playing a certain move
    short bestMove = 0; //stores the move the gives the highest probability of wining
    point computerMove; //converts the best move to a point to store in computer moves

    //get all the free moves on the board and calculate the possibility of a win if the computer plays that move
    vector<short> possibleMoves(getPossibleMoves(Hex::moves));
    for (short newMove : possibleMoves)
    {
        newProb = monteCarlo(newMove);

        //get the highest probability and the best move
        if (newProb > highestProb)
        {
            highestProb = newProb;
            bestMove = newMove;
        }
    }


    //add the best move to computer moves and return the value
    computerMove = convertToPoint(matrixSize, bestMove);
    Hex::addMove(*this, computerMode, computerMove);
    return computerMove;
}

double Hex::monteCarlo(short newMove)
{
    short const TRIALS = 1500; // number of boards generated with random moves
    short numWins = 0; //calculates the number of times the computer won

    //create a new game adding all the previous moves of the computer and player
    Hex newGame(*this);

    //add the newMove to the computer moves
    Hex::addMove(newGame, computerMode, convertToPoint(matrixSize, newMove));

    //get all the possible moves and store them
    vector<short> possibleMoves = getPossibleMoves(newGame.moves);
    short freeMovesSize = possibleMoves.size();
    short halfFreeMoves = freeMovesSize / 2;

    //fill boards with random moves and compute the probability of a win with the new move
    for (short i = 0; i < TRIALS; ++i)
    {
        //shuffle the moves and fill half with computer moves and the other half with player moves
        shuffle(possibleMoves.begin(), possibleMoves.end(), default_random_engine(time(0)));

        for (int j = 0; j < freeMovesSize; ++j)
        {
            if (j < halfFreeMoves)
            {
                Hex::addMove(newGame, playerMode, convertToPoint(matrixSize, possibleMoves[j]));
            }
            else
            {
                Hex::addMove(newGame, computerMode, convertToPoint(matrixSize, possibleMoves[j]));
            }
        }

        //find out if the the computer won the game and update variables accordingly
        if(whoWon(newGame, newGame.player, newGame.computer) == Hex::computerMode)
        {
            ++numWins;
        }


        //reset newGame to the initial game and add the new move
        newGame = *this;
        Hex::addMove(newGame, computerMode, convertToPoint(matrixSize, newMove));

    }

    //calculate the probability of a win and return the value
    return (numWins*1.0)/TRIALS;

}

void Hex::operator=(const Hex& other)
{
    Graph::operator=(static_cast<Graph>(other));
    Hex::hexNeighbours = other.hexNeighbours;
    Hex::matrixSize = other.matrixSize;
    Hex::playerMode = other.playerMode;
    Hex::computerMode = other.computerMode;
    Hex::hexHasEdge = other.hexHasEdge;
    Hex::player = other.player;
    Hex::computer = other.computer;
    Hex::moves = other.moves;
}
