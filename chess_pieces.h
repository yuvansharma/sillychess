#ifndef _CHESS_PIECES_H_
#define _CHESS_PIECES_H_

#include <iostream>
#include <map>
#include <vector>

#include "utf8_codepoint.h"
#include "chess_board.h"

using std::istream;
using std::map;
using std::ostream;
using std::vector;

class ChessPiece {
public:
    const UTF8CodePoint utf8_codepoint;
    const Team team;

    ChessPiece(UTF8CodePoint cp, Team team) : utf8_codepoint(cp), team(team) {}

    virtual ~ChessPiece() {}

    virtual void get_moves(const Board& board, Cell from, vector<Move>& moves) const = 0;
    virtual void make_move(Board& board, Move move) const = 0;

    bool is_opposite_team(const ChessPiece& other) const;

    bool operator==(const ChessPiece& other) const;
    bool operator!=(const ChessPiece& other) const;

    friend ostream& operator<<(ostream& os, const ChessPiece& p);
};

ostream& operator<<(ostream& os, const ChessPiece& p);

class EmptySpace : public ChessPiece {
public:
    EmptySpace() : ChessPiece('.', NONE) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override {}
    void make_move(Board& board, Move move) const override {}
};


class SimpleChessPiece : public ChessPiece {
public:
    SimpleChessPiece(UTF8CodePoint cp, Team team) : ChessPiece(cp, team) {}
    void make_move(Board& board, Move move) const;
};

class King : public SimpleChessPiece {
public:
    King(UTF8CodePoint cp, Team team) : SimpleChessPiece(cp, team) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

class Queen : public SimpleChessPiece {
public:
    Queen(UTF8CodePoint cp, Team team) : SimpleChessPiece(cp, team) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

class Bishop : public SimpleChessPiece {
public:
    Bishop(UTF8CodePoint cp, Team team) : SimpleChessPiece(cp, team) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

class Knight : public SimpleChessPiece {
public:
    Knight(UTF8CodePoint cp, Team team) : SimpleChessPiece(cp, team) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

class Rook : public SimpleChessPiece {
public:
    Rook(UTF8CodePoint cp, Team team) : SimpleChessPiece(cp, team) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

class Pawn : public SimpleChessPiece {
    int y_move_steps;
public:
    Pawn(UTF8CodePoint cp, Team team, int y_move_steps)
        : SimpleChessPiece(cp, team), y_move_steps(y_move_steps) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

/*
*
* A BackBencher can move to any cell in any of the rows behind it, and can 
* remove an opponent's piece in the process.
* While moving forward, a BackBencher behaves exactly like a regular Pawn.
*
*/

class BackBencher : public SimpleChessPiece {
    int forward_steps;
public:
    BackBencher(UTF8CodePoint cp, Team team, int forward_steps) : SimpleChessPiece(cp, team), forward_steps(forward_steps) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

/* A Mouse likes to hide: it can move to the two corners of the row its in and the
* surrounding rows, and to the cells adjacent to those corners. It can also retreat
to the safety of its team's corners at anytime. If you really want to trouble it, though,
it can also move forward, but only to the cell directly in front of it. If an opponent's
piece is in front of a Mouse, or in any of the Cells the Mouse can move to, it gets nibbled to death!
*/
class Mouse : public SimpleChessPiece {
public:
    Mouse(UTF8CodePoint cp, Team team) : SimpleChessPiece(cp, team) {}
    void get_moves(const Board& board, Cell from, vector<Move>& moves) const override;
};

// `extern` is used to declare the variables here, without defining them
// The actual variables/objects are defined in the corresponding .cpp file.
extern const EmptySpace EMPTY_SPACE;
extern const King WHITE_KING;
extern const King BLACK_KING;
extern const Queen WHITE_QUEEN;
extern const Queen BLACK_QUEEN;
extern const Bishop WHITE_BISHOP;
extern const Bishop BLACK_BISHOP;
extern const Knight WHITE_KNIGHT;
extern const Knight BLACK_KNIGHT;
extern const Rook WHITE_ROOK;
extern const Rook BLACK_ROOK;
extern const Pawn WHITE_PAWN;
extern const Pawn BLACK_PAWN;

extern const BackBencher WHITE_BACKBENCHER;
extern const BackBencher BLACK_BACKBENCHER;

extern const Mouse WHITE_MOUSE;
extern const Mouse BLACK_MOUSE;

extern const map<UTF8CodePoint, const ChessPiece*> ALL_CHESS_PIECES;

#endif  // _CHESS_PIECES_H_#pragma once
