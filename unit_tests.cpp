#include <iostream>
#include <vector>
#include <sstream>
#include "assert.h"
#include "chess_board.h"
#include "chess_pieces.h"
#include "chess_player.h"

using namespace std;

void play_chess_one_turn(Board& board, Player& player) {
    cout << board << endl;
    cout << player.name() << "'s turn." << endl;
    vector<Move> moves = board.get_moves();
    Move move;
    while (true) {
        move = player.get_move(board, moves);
        if (find(moves.begin(), moves.end(), move) != moves.end()) {
            break;
        }
    }
    cout
        << player.name() << " chose to move " << board[move.from]
        << " from " << move.from << " to " << move.to << " ("
        << board[move.to] << ")\n\n";
    board.make_move(move);
}

Team play_one_chess_game(Player& white_player, Player& black_player) {
    Board board;
    vector<Move> moves;
    while (true) {
        play_chess_one_turn(board, white_player);
        if (board.winner() != NONE) {
            break;
        }
        play_chess_one_turn(board, black_player);
        if (board.winner() != NONE) {
            break;
        }
    }
    Team winner = board.winner();
    cout << team_name(winner) << " won!\n";
    return winner;
}

void assert_equals(bool condition, string message)                                 
{                                                               
    if (!condition)
    {
        cerr << message << endl;
        assert(condition);

    }
    }
/*

   abcdefgh
 8 ♜♞♝♛♚♝♞♜ 7
 7 ♟♟♟♟♟♟♟♟ 6
 6 ........ ......5
 5 ........ ......4
 4 ........ ......3
 3 ........ ......2
 2 ♙♙♙♙♙♙♙♙ 1
 1 ♖♘♗♕♔♗♘♖ 0
   0 1 2 3 4 5 6 7
   a b c d e f g h



*/
/*
b1a3 = 1,0 to 0,2
b1c3 = 1,0 to 2,2
g1f3 = 6,0 to 5,2
g1h3 = 6,0 to 7,2
a2a3 = 0,1 to 0,2
b2b3 = 1,1 to 1,2
c2c3 = 2,1 to 2,2
d2d3 = 3,1 to 3,2
e2e3 = 4,1 to 4,2
f2f3 = 5,1 to 5,2
g2g3 = 6,1 to 6,2
h2h3 = 7,1 to 7,2
*/
vector<Move> initial_moves()
{
    vector<Move> moves = { Move(Cell(1, 0), Cell(0, 2)),
                           Move(Cell(1, 0), Cell(2, 2)),
                           Move(Cell(6, 0), Cell(5, 2)),
                           Move(Cell(6, 0), Cell(7, 2)),
                           Move(Cell(0, 1), Cell(0, 2)),
                           Move(Cell(1, 1), Cell(1, 2)),
                           Move(Cell(2, 1), Cell(2, 2)),
                           Move(Cell(3, 1), Cell(3, 2)),
                           Move(Cell(4, 1), Cell(4, 2)),
                           Move(Cell(5, 1), Cell(5, 2)),
                           Move(Cell(6, 1), Cell(6, 2)),
                           Move(Cell(7, 1), Cell(7, 2)),
    };
    return moves;
}
void test_reset_board_moves()
{
	Board board;
    vector<Move> expected_moves = initial_moves();
	vector<Move> actual_moves = board.get_moves();
    assert_equals(expected_moves == actual_moves, "Expected moves of starting board not equal to actual moves, error in test_board");

}

void test_strategies()
{
    RandomPlayer r1(WHITE);
    CapturePlayer c1(BLACK);
    CheckMateCapturePlayer c2(BLACK);
    CapturePlayer c3(WHITE);

    int counter1 = 0, counter2 = 0;
    for (int i = 0; i < 100; ++i) // reduce from 10000
    {
        if (play_one_chess_game(r1, c1) == BLACK)
            ++counter1;
        if (play_one_chess_game(c3, c2) == WHITE)
            ++counter2;


    }
    
    bool condition1 = false, condition2 = false;
    if (!(counter1 < 90))
        condition1 = true;
    if (!(counter2 < 35 || counter2>55))
        condition2 = true;
    assert_equals(condition1, "Result of Random Player as WHITE and Capture Player as BLACK not equal to expected in test_strategies()");
    assert_equals(condition2, "Result of Capture Player as WHITE and Checkmate Capture Player as BLACK not equal to expected in test_strategies()");

        
}

int main()
{
    test_reset_board_moves();
    test_strategies();
}