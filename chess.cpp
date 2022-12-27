#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "chess_pieces.h"
#include "chess_board.h"
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

int main(int argc, const char* argv[]) {
    AIPlayer white1(WHITE);
    CheckMateCapturePlayer black1(BLACK);
    AIPlayer black2(BLACK);
    CheckMateCapturePlayer white2(WHITE);
    HumanPlayer human(WHITE);

    play_one_chess_game(human, black2);

    return 0;


    int counter1 = 0;
    int counter2 = 0;
    int num_games = 100;
    for (int i = 1; i <= num_games; ++i)
    {
        Team x = play_one_chess_game(white1, black1);
        Team y = play_one_chess_game(white2, black2);
        if (x == WHITE)
            ++counter1;
        if (y == BLACK)
            ++counter2;
        cout << endl;
    }
    cout << "As white AI Player won: " << counter1 << "/" << num_games << endl;
    cout << "As black AI Player won: " << counter2 << "/" << num_games << endl;

    return 0;
}