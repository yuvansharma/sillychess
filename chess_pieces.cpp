#include "utf8_codepoint.h"
#include "chess_pieces.h"


bool ChessPiece::is_opposite_team(const ChessPiece& other) const {
    return (team == WHITE && other.team == BLACK) || (team == BLACK && other.team == WHITE);
}

bool ChessPiece::operator==(const ChessPiece& other) const {
    return utf8_codepoint == other.utf8_codepoint;
}
bool ChessPiece::operator!=(const ChessPiece& other) const {
    return utf8_codepoint != other.utf8_codepoint;
}

ostream& operator<<(ostream& os, const ChessPiece& p) {
    return os << p.utf8_codepoint;
}

void SimpleChessPiece::make_move(Board& board, Move move) const {
    board.make_classical_chess_move(move);
}

void King::get_moves(const Board& board, Cell from, vector<Move>& moves) const {
    for (int x = from.x - 1; x < from.x + 2; ++x) {
        for (int y = from.y - 1; y < from.y + 2; ++y) {
            Cell to(x, y);
            if (from != to && board.contains(to)
                && (board[to] == EMPTY_SPACE || is_opposite_team(board[to]))) {
                moves.emplace_back(from, to);
            }
        }
    }
}

void Queen::get_moves(const Board& board, Cell from, vector<Move>& moves) const {
    // The 8 directions a queen can go...
    Cell directions[] = {
      {-1,  1}, {0,  1}, {1,  1},
      {-1,  0},          {1,  0},
      {-1, -1}, {0, -1}, {1, -1},
    };
    for (Cell direction : directions) {
        for (int steps = 1; ; ++steps) {
            Cell to(from.x + steps * direction.x, from.y + steps * direction.y);
            if (!board.contains(to)) {
                break;
            }
            const ChessPiece& piece = board[to];
            if (piece == EMPTY_SPACE) {
                moves.emplace_back(from, to);
            }
            else if (is_opposite_team(piece)) {
                moves.emplace_back(from, to);
                break;
            }
            else {  // if (team == piece.team)
                break;
            }
        }
    }
}

void Bishop::get_moves(const Board& board, Cell from, vector<Move>& moves) const {
    // The 4 directions a bishop can go...
    Cell directions[] = {
      {-1,  1}, {1,  1},
      {-1, -1}, {1, -1},
    };
    for (Cell direction : directions) {
        for (int steps = 1; ; ++steps) {
            Cell to(from.x + steps * direction.x, from.y + steps * direction.y);
            if (!board.contains(to)) {
                break;
            }
            const ChessPiece& piece = board[to];
            if (piece == EMPTY_SPACE) {
                moves.emplace_back(from, to);
            }
            else if (is_opposite_team(piece)) {
                moves.emplace_back(from, to);
                break;
            }
            else {  // if (team == piece.team)
                break;
            }
        }
    }
}

void Knight::get_moves(const Board& board, Cell from, vector<Move>& moves) const {
    Cell jumps[] = {
           {-1,  2}, {1,  2},
      {-2,  1},          {2,  1},
      {-2, -1},          {2, -1},
           {-1, -2}, {1, -2},
    };
    for (Cell jump : jumps) {
        Cell to(from.x + jump.x, from.y + jump.y);
        if (board.contains(to)) {
            const ChessPiece& piece = board[to];
            if (piece == EMPTY_SPACE || is_opposite_team(piece)) {
                moves.emplace_back(from, to);
            }
        }
    }
}

void Rook::get_moves(const Board& board, Cell from, vector<Move>& moves) const {
    // The 4 directions a rook can go...
    Cell directions[] = {
                {0,  1},
      {-1,  0},          {1,  0},
                {0, -1},
    };
    for (Cell direction : directions) {
        for (int steps = 1; ; ++steps) {
            Cell to(from.x + steps * direction.x, from.y + steps * direction.y);
            if (!board.contains(to)) {
                break;
            }
            const ChessPiece& piece = board[to];
            if (piece == EMPTY_SPACE) {
                moves.emplace_back(from, to);
            }
            else if (is_opposite_team(piece)) {
                moves.emplace_back(from, to);
                break;
            }
            else {  // if (team == piece.team)
                break;
            }
        }
    }
}

void Pawn::get_moves(const Board& board, Cell from, vector<Move>& moves) const {
    Cell to = Cell(from.x, from.y + y_move_steps);
    if (board.contains(to) && board[to] == EMPTY_SPACE) {
        moves.emplace_back(from, to);
    }

    to = Cell(from.x - 1, from.y + y_move_steps);
    if (board.contains(to) && is_opposite_team(board[to])) {
        moves.emplace_back(from, to);
    }

    to = Cell(from.x + 1, from.y + y_move_steps);
    if (board.contains(to) && is_opposite_team(board[to])) {
        moves.emplace_back(from, to);
    }
}

/*
*
* A BackBencher can move to any cell in any of the rows behind it, and can
* remove an opponent's piece in the process.
* While moving forward, a BackBencher behaves exactly like a regular Pawn.
*
*/

void BackBencher::get_moves(const Board& board, Cell from, vector<Move>& moves) const {
   
    Cell to = Cell(from.x, from.y + forward_steps);
    if (board.contains(to) && board[to] == EMPTY_SPACE) {
        moves.emplace_back(from, to);
    }

    to = Cell(from.x - 1, from.y + forward_steps);
    if (board.contains(to) && is_opposite_team(board[to])) {
        moves.emplace_back(from, to);
    }

    to = Cell(from.x + 1, from.y + forward_steps);
    if (board.contains(to) && is_opposite_team(board[to])) {
        moves.emplace_back(from, to);
    }
    // black is at   top  of the board
    // white is at bottom of the board
    if (team == WHITE)
    {
        if (from.y == 0) // the BackBencher is already all the way at the back of the board
            return;
        int y_value = from.y-1;
        Cell to;
        while (y_value >= 0)
        {
            for (int i = 0; i < 8; ++i)
            {
                to = Cell(i, y_value);
                if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to]))) // only a valid move if cell is empty/has opponent's piece
                    moves.emplace_back(from, to);
            }
            --y_value;
        }
    }
    if (team == BLACK)
    {
        if (from.y == 8)
            return;
        int y_value = from.y + 1;
        Cell to;
        while (y_value <= 8) // the BackBencher is already all the way at the back of the board
        {
            for (int i = 0; i < 8; ++i)
            {
                to = Cell(i, y_value);
                if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
                    moves.emplace_back(from, to);
            }
            ++y_value;
        }
    }
    
}

/* A Mouse likes to hide: it can move to the two corners of the row its in and the
* surrounding rows, and to the cells adjacent to those corners. It can also retreat
to the safety of its team's corners at anytime. If you really want to trouble it, though, 
it can also move forward, but only to the cell directly in front of it. If an opponent's
piece is in front of a Mouse, or in any of the Cells the Mouse can move to, it gets nibbled to death!
*/

void Mouse::get_moves(const Board& board, Cell from, vector<Move>& moves) const
{
    int forward = 0;
    if (team == WHITE)
        forward = 1;
    else if (team == BLACK)
        forward = -1;
    Cell to = Cell(from.x, from.y + forward);
    if (board.contains(to)&& (board[to] == EMPTY_SPACE || is_opposite_team(board[to]))) {
        moves.emplace_back(from, to);
    }
    // add corners and adjacent cells for the three rows

    to = Cell(0, from.y);
    if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to])))       
        moves.emplace_back(from, to);

    to = Cell(7, from.y);
    if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
        moves.emplace_back(from, to);

    to = Cell(0, from.y + 1);
    if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
        moves.emplace_back(from, to);

    to = Cell(7, from.y + 1);
    if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
        moves.emplace_back(from, to);

    to = Cell(0, from.y - 1);
    if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
        moves.emplace_back(from, to);

    to = Cell(7, from.y - 1);
    if (board.contains(to) && (board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
        moves.emplace_back(from, to);

    Cell to2;
    if (team == WHITE) {
        to  = Cell(0, 0);
        to2 = Cell(7, 0);
       if((board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
        moves.emplace_back(from, to);
       if(board[to2] == EMPTY_SPACE || is_opposite_team(board[to2]))
        moves.emplace_back(from, to2);

    }
    if (team == BLACK)
    {
        to  = Cell(0, 7);
        to2 = Cell(7, 7);
        if ((board[to] == EMPTY_SPACE || is_opposite_team(board[to])))
            moves.emplace_back(from, to);
        if (board[to2] == EMPTY_SPACE || is_opposite_team(board[to2]))
            moves.emplace_back(from, to2);
    }
}



const EmptySpace EMPTY_SPACE;
const King WHITE_KING(U'♔', WHITE);
const King BLACK_KING(U'♚', BLACK);
const Queen WHITE_QUEEN(U'♕', WHITE);
const Queen BLACK_QUEEN(U'♛', BLACK);
const Bishop WHITE_BISHOP(U'♗', WHITE);
const Bishop BLACK_BISHOP(U'♝', BLACK);
const Knight WHITE_KNIGHT(U'♘', WHITE);
const Knight BLACK_KNIGHT(U'♞', BLACK);
const Rook WHITE_ROOK(U'♖', WHITE);
const Rook BLACK_ROOK(U'♜', BLACK);
const Pawn WHITE_PAWN(U'♙', WHITE, 1);
const Pawn BLACK_PAWN(U'♟', BLACK, -1);

const BackBencher WHITE_BACKBENCHER(U'⛉', WHITE, 1);
const BackBencher BLACK_BACKBENCHER(U'⛊', BLACK, -1);

const Mouse WHITE_MOUSE(U'🐁', WHITE);
const Mouse BLACK_MOUSE(U'🐀', BLACK);

const map<UTF8CodePoint, const ChessPiece*> ALL_CHESS_PIECES = {
  {EMPTY_SPACE.utf8_codepoint, &EMPTY_SPACE},
  {WHITE_KING.utf8_codepoint, &WHITE_KING},
  {BLACK_KING.utf8_codepoint, &BLACK_KING},
  {WHITE_QUEEN.utf8_codepoint, &WHITE_QUEEN},
  {BLACK_QUEEN.utf8_codepoint, &BLACK_QUEEN},
  {WHITE_BISHOP.utf8_codepoint, &WHITE_BISHOP},
  {BLACK_BISHOP.utf8_codepoint, &BLACK_BISHOP},
  {WHITE_KNIGHT.utf8_codepoint, &WHITE_KNIGHT},
  {BLACK_KNIGHT.utf8_codepoint, &BLACK_KNIGHT},
  {WHITE_ROOK.utf8_codepoint, &WHITE_ROOK},
  {BLACK_ROOK.utf8_codepoint, &BLACK_ROOK},
  {WHITE_PAWN.utf8_codepoint, &WHITE_PAWN},
  {BLACK_PAWN.utf8_codepoint, &BLACK_PAWN},
    {WHITE_BACKBENCHER.utf8_codepoint, &WHITE_BACKBENCHER},
    {BLACK_BACKBENCHER.utf8_codepoint, &BLACK_BACKBENCHER},
    {WHITE_MOUSE.utf8_codepoint, &WHITE_MOUSE},
    {BLACK_MOUSE.utf8_codepoint, &BLACK_MOUSE}
};