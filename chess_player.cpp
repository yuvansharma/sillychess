#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include "chess_board.h"
#include "chess_pieces.h"
#include "chess_player.h"

using std::cin;
using std::cout;
using std::endl;
using std::shuffle;
using std::vector;

const int POS_INF = 99999999;
const int NEG_INF = -99999999;
const char* Player::name() const {
    return team_name(team);
}


RandomPlayer::RandomPlayer(Team team) : Player(team) {
    // Initialize the pseudo-random number generator based on the current time,
    // so it chooses different numbers when you run the code at different times.
    random_number_generator.seed(
        std::chrono::system_clock::now().time_since_epoch().count());
}

Move RandomPlayer::get_move(const Board& board, const vector<Move>& moves) const {
    return moves[random_number_generator() % moves.size()];
}

AIPlayer::AIPlayer(Team team) : Player(team) {
    // Initialize the pseudo-random number generator based on the current time,
    // so it chooses different numbers when you run the code at different times.
    random_number_generator.seed(
        std::chrono::system_clock::now().time_since_epoch().count());
}
HumanPlayer::HumanPlayer(Team team) : Player(team) {}

Move HumanPlayer::get_move(const Board& board, const vector<Move>& moves) const {
    Move move;
    while (true) {
        cout << "What's your move?: ";
        cin >> move;
        cout << endl;
        if (find(moves.begin(), moves.end(), move) != moves.end()) {
            break;
        }
        cout << move << " is not a valid move! Please choose one of the following moves: \n";
        for (Move valid_move : moves) {
            cout << valid_move << ' ';
        }
        cout << endl;
    }
    return move;
}

CapturePlayer::CapturePlayer(Team team) : Player(team) {
    // Initialize the pseudo-random number generator based on the current time,
    // so it chooses different numbers when you run the code at different times.
    random_number_generator.seed(
        std::chrono::system_clock::now().time_since_epoch().count());
}

Move CapturePlayer::get_move(const Board& board, const vector<Move>& moves) const {
    vector<Move> shuffled_moves = moves;
    shuffle(shuffled_moves.begin(), shuffled_moves.end(), random_number_generator);
    for (Move move : shuffled_moves) {
        if (board[move.from].is_opposite_team(board[move.to])) {
            return move;
        }
    }
    return shuffled_moves[0];
}

CheckMateCapturePlayer::CheckMateCapturePlayer(Team team) : Player(team) {
    // Initialize the pseudo-random number generator based on the current time,
    // so it chooses different numbers when you run the code at different times.
    random_number_generator.seed(
        std::chrono::system_clock::now().time_since_epoch().count());
}

Move CheckMateCapturePlayer::get_move(const Board& board, const vector<Move>& moves) const {
    vector<Move> shuffled_moves = moves;
    shuffle(shuffled_moves.begin(), shuffled_moves.end(), random_number_generator);
    for (Move move : shuffled_moves) {
        if (board[move.from].is_opposite_team(board[move.to]) && (board[move.to] == WHITE_KING || board[move.to] == BLACK_KING)) {
            return move;
        }
    }
    for (Move move : shuffled_moves) {
        if (board[move.from].is_opposite_team(board[move.to])) {
            return move;
        }
    }
    return shuffled_moves[0];
}

Move AIPlayer::get_move(const Board& board, const vector<Move>& moves) const
{   
 
    vector<Move> shuffled_moves = moves;

    if (shuffled_moves.size() > 0) {
        Move best_move = shuffled_moves[0];
        int best_score = team == WHITE ? NEG_INF : POS_INF;
        for (Move move : shuffled_moves)
        {
            if (team == WHITE)
            {
                int x = minimax(board, move, 4, NEG_INF, POS_INF, false);
              
                if (x > best_score)
                {
                    best_move = move;
                    best_score = x;
                }
            }
            else
            {
                int x = minimax(board, move, 4, NEG_INF, POS_INF, true);
                if (x < best_score)
                {
                    best_move = move;
                    best_score = x;

                }
            }
        }
        return best_move;
    }
    return shuffled_moves[0];
}



int AIPlayer::minimax(const Board& b, Move move, int depth, int alpha, int beta, bool white) const
{
    Board b2 = b;
    b2.make_move(move);
 
    if (depth == 1)
    {
     return eval(b2);
    }

    int eval;

    if (white)
    {
        int maxEval = NEG_INF; // representative of - infinity

            for (Move m : b2.get_moves())
            {
                eval = 0;
                eval = minimax(b2, m, depth - 1, alpha, beta, false);       
                maxEval = maxEval > eval ? maxEval : eval;
                alpha = alpha > eval ? alpha : eval;
              if (beta <= alpha)
                   break;
            }
        
        return maxEval;
    }
    else
    {
        int minEval = POS_INF; // representative of + infinity
  
            for (Move m : b2.get_moves())
            {
                eval = minimax(b2, m, depth - 1, alpha, beta, true);

                minEval = minEval < eval ? minEval : eval;
                beta = beta < eval ? beta : eval;
               if (beta <= alpha)
                   break;
            }
        return minEval;
    }
}

int AIPlayer::eval(const Board& b) const
{
    int evaluation = 0;

    Cell c;
    int sign = 0;


    for(int j = 0; j < 8; ++j)
    {
        for (int i = 0; i < 8; ++i)
        {

            c = Cell(i, j);

            sign = (b[c].team == WHITE) ? 1 : -1; // for empty spaces, it will be -1 but it doesn't matter since their value is 0

            evaluation = evaluation + value(b[c]) * sign;

        }
    }
     
    return evaluation;
}

int AIPlayer::value(const ChessPiece& p) const
{
    if (p == EMPTY_SPACE)  
        return 0;
    if (p == BLACK_PAWN || p == WHITE_PAWN)
        return 1;
    if (p == BLACK_KNIGHT || p == WHITE_KNIGHT || p == BLACK_BISHOP || p == WHITE_BISHOP)
        return 3;
    if (p == BLACK_ROOK || p == WHITE_ROOK)
        return 5;
    if (p == BLACK_QUEEN || p == WHITE_QUEEN)
        return 9;
    if (p == WHITE_KING || p == BLACK_KING)
        return 1000;
    return 0;
}







































/*
bool AIPlayer::good_move(const Move move, const Board& board) const
{
    if (team == WHITE) // defending tactics
    {
        if (move.to.y < 4 && board[move.from].is_opposite_team(board[move.to]))
            return true;
    }
    else if (team == BLACK)
    {
        if (move.to.y >= 4 && board[move.from].is_opposite_team(board[move.to]))
            return true;
    }
    if (board[move.from].is_opposite_team(board[move.to]) && (board[move.to] == WHITE_KING || board[move.to] == BLACK_KING))
        return true;

    if (board[move.from].is_opposite_team(board[move.to]) && is_more_value(board[move.to], board[move.from]))
        return true;

    return false;
}
bool AIPlayer::is_more_value(const ChessPiece& p1, const ChessPiece& p2) const {

    if (p2 == WHITE_KING || p2 == BLACK_KING || p2 == WHITE_QUEEN || p2 == BLACK_QUEEN)
        return true;
    if ((p1 == WHITE_PAWN || p1 == BLACK_PAWN) && (p2 != WHITE_PAWN && p2 != BLACK_PAWN))
        return true;
    if ((p1 == WHITE_KNIGHT || p1 == BLACK_KNIGHT) && (p2 == WHITE_ROOK || p2 == BLACK_ROOK))
        return true;
    if ((p1 == WHITE_BISHOP || p1 == BLACK_BISHOP) && (p2 == WHITE_ROOK || p2 == BLACK_ROOK))
        return true;
    if ((p1 == WHITE_BISHOP || p1 == BLACK_BISHOP) && (p2 == WHITE_KNIGHT || p2 == BLACK_KNIGHT))
        return true;


    return false;
}*/