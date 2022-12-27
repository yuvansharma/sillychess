#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "utf8_codepoint.h"
#include "chess_pieces.h"
#include "chess_board.h"

using std::endl;
using std::istream;
using std::map;
using std::ostream;
using std::out_of_range;
using std::stringstream;
using std::vector;
using std::string;
using std::runtime_error;
using std::ios;
using std::getline;
using std::cout;

const char* team_name(Team team) {
    switch (team) {
    case WHITE:
        return "White";
    case BLACK:
        return "Black";
    case NONE:
        return "None";
    }
    return "UNKNOWN";
}


bool Cell::operator==(Cell other) const {
    return x == other.x && y == other.y;
}

bool Cell::operator!=(Cell other) const {
    return !(*this == other);
}

bool Move::operator==(Move other) const {
    return to == other.to && from == other.from;
}

bool Move::operator!=(Move other) const {
    return !(*this == other);
}


ostream& operator<<(ostream& os, const Cell& cell) {
    return os << static_cast<char>(cell.x + 'a') << cell.y + 1;
}
istream& operator>>(istream& is, Cell& cell) {
    char x_plus_a;
    int y_plus_1;
    is >> x_plus_a >> y_plus_1;
    cell.x = x_plus_a - 'a';
    cell.y = y_plus_1 - 1;
    return is;
}

ostream& operator<<(ostream& os, const Move& move) {
    return os << move.from << move.to;
}
istream& operator>>(istream& is, Move& move) {
    return is >> move.from >> move.to;
}

Board::Board() {
    reset_board();
}

const ChessPiece& Board::operator[](Cell cell) const {
    return *board2[cell.y][cell.x];
}

/*
vector<vector<int>> ints;

    for (int i = 0; i < 5; ++i)
    {
        vector<int> x;
        ints.push_back(x);
        for (int j = 0; j < 5; ++j)
        {
            ints[i].push_back(j);
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            cout << ints[i][j] << " ";
        }
        cout << endl;
    }
*/

void Board::reset_board() {
    for (int y = 0; y < 8; ++y) {
        vector<const ChessPiece*> x;
        board2.push_back(x);
        for (int x = 0; x < 8; ++x) {
            board2[y].push_back(& EMPTY_SPACE);
        }
    }

    for (int x = 0; x < 8; ++x) {
        board2[1][x] = &WHITE_PAWN;
        board2[6][x] = &BLACK_PAWN;
    }

    board2[0][0] = &WHITE_ROOK;
    board2[0][1] = &WHITE_KNIGHT;
    board2[0][2] = &WHITE_BISHOP;
    board2[0][3] = &WHITE_QUEEN;
    board2[0][4] = &WHITE_KING;
    board2[0][5] = &WHITE_BISHOP;
    board2[0][6] = &WHITE_KNIGHT;
    board2[0][7] = &WHITE_ROOK;

    board2[7][0] = &BLACK_ROOK;
    board2[7][1] = &BLACK_KNIGHT;
    board2[7][2] = &BLACK_BISHOP;
    board2[7][3] = &BLACK_QUEEN;
    board2[7][4] = &BLACK_KING;
    board2[7][5] = &BLACK_BISHOP;
    board2[7][6] = &BLACK_KNIGHT;
    board2[7][7] = &BLACK_ROOK;

    current_teams_turn = WHITE;
}

vector<Move> Board::get_moves() const {
    vector<Move> moves;
    for (int y = 0; y < board2.size(); ++y) {
        for (int x = 0; x < board2[y].size(); ++x) {
            if (board2[y][x]->team == current_teams_turn) {
                board2[y][x]->get_moves(*this, Cell(x, y), moves);
            }
        }
    }
    for (Move move : moves) {
        if (!contains(move.to) || !contains(move.from)) {
            stringstream err_msg;
            err_msg << "Board::get_moves got a move that moves to or from a cell that is not on the board: " << move;
            throw out_of_range(err_msg.str());
        }
    }
    return moves;
}

// This function represents how most classical chess ALL_CHESS_PIECES would move.
// This also allows us to add support for more complex "moves", like a pawn
// getting to the end of the board and turning into a queen or some other type
// of piece.
// If we allow the chess piece that's moving to define the move, then we can
// add really interesting custom ALL_CHESS_PIECES that are nothing like normal ALL_CHESS_PIECES!
void Board::make_classical_chess_move(Move move) {
    board2[move.to.y][move.to.x] = board2[move.from.y][move.from.x];
    board2[move.from.y][move.from.x] = &EMPTY_SPACE;
    current_teams_turn = current_teams_turn == WHITE ? BLACK : WHITE;
}

void Board::make_move(Move move) {
    if (!contains(move.to) || !contains(move.from)) {
        stringstream err_msg;
        err_msg << "Board::make_move called with a move that moves to or from a cell that is not on the board: " << move;
        throw out_of_range(err_msg.str());
    }
    board2[move.from.y][move.from.x]->make_move(*this, move);
}

bool Board::contains(Cell cell) const { // CHANGE THIS
    return cell.x >= 0 && cell.x < board2[0].size() && cell.y >= 0 && cell.y < board2.size();
}

Team Board::winner() const {
    bool found_white_king = false, found_black_king = false;
    for (int y = 0; y < board2.size(); ++y) { //CHANGE THIS
        for (int x = 0; x < board2[y].size(); ++x) {
            if (board2[y][x] == &WHITE_KING) {
                found_white_king = true;
            }
            else if (board2[y][x] == &BLACK_KING) {
                found_black_king = true;
            }
        }
    }
    if (!found_white_king) {
        return BLACK;
    }
    if (!found_black_king) {
        return WHITE;
    }
    return NONE;
}

ostream& operator<<(ostream& os, const Board& board) {
    os << "   ";
        //abcdefgh\n"; // CHANGE THIS
    for (int x = 0; x < board.board2[0].size(); ++x)
    {
        os << static_cast<char>('a' + x);
    }
    os << '\n';
    for (int y = board.board2.size()-1; y >= 0; --y) {
        if(y>=9)
            os << (y + 1) << ' ';
        else
            os << ' ' << (y + 1) << ' ';


        for (int x = 0; x < board.board2[y].size(); ++x) {
            os << board[Cell(x, y)];
        }
       // if (y >= 10)
        //    os << y + 1 << '\n';
       // else
           os << ' ' << (y + 1) << '\n';
    }
    os << "   ";
    for (int x = 0; x < board.board2[0].size(); ++x)
    {
        os << static_cast<char>('a' + x);
    }
    os << "\n";
    return os;
}


/*
   abcdefg     h
 8 ♜♞♝♛♚♝♞♜ 8
 7 ♟♟♟♟♟♟♟♟ 7
 6 ........ 6
 5 ........ 5
 4 ........ 4
 3 ........ 3
 2 ♙♙♙♙♙♙♙♙ 2
 1 ♖♘♗♕♔♗♘♖ 1
   abcdefgh


*/

/*

   ab
 4 ♛♙ 4
 3 .. 3
 2 ♟. 2
 1 ♕♔ 1
   ab

*/

istream& operator>>(istream& is, Board& board)
{
    string s;
    stringstream st;
    UTF8CodePoint utf;




    board.board2.clear();

    is.seekg(0, ios::beg);

    char c;
    int x_max = 0; // number of columns
    int y_max = 0; // number of rows

    is.seekg(3, ios::cur);
    is.get(c);

    while (c != '\n')
    {
        is.get(c);
        ++x_max;
    }

    // now count number of rows by counting frequency of '\n'

    is.seekg(0, ios::beg);

    getline(is, s);

    while (is.peek() != EOF)
    {
        is.get(c);
        if (c == '\n')
            ++y_max;
    }
    y_max = y_max - 1; // subtract one since it will also read \n of the last line of the file, which has the column labels abcdefg...



    is.seekg(0, ios::beg);
    getline(is, s);

    for (int i = 0; i < y_max; ++i)
    {
        vector<const ChessPiece*> x;
        board.board2.push_back(x);
        for (int j = 0; j < x_max; ++j)
        {
            board.board2[i].push_back(&EMPTY_SPACE);
        }
    }

    is.seekg(0, ios::beg);
    getline(is, s);

    for (int i = y_max - 1; i >= 0; --i)
    {
        is.seekg(3, ios::cur);

        for (int j = 0; j < x_max; ++j)
        {
            is >> utf;
            board.board2[i][j] = (ALL_CHESS_PIECES.at(utf));

        }
        getline(is, s);

    }
    return is;
}

