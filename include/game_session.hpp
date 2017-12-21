#include "game_board.hpp"

#include <iostream>
#include <vector>

enum game_status {tie, xwins, owins};

template<class PlayerX, class PlayerO>
class game_session {
  PlayerX playerx;
  PlayerO playero;
  std::vector<game_board> boards{ game_board() };
  std::vector<int> moves;
  bool done = false, tie = false;
  bool game_won() {
    auto& board = this->current_board();
    // rows
    for(int r=0; r<3; ++r) {
      if((board(r, 0) == board(r, 1)) and (board(r, 1) == board(r, 2)))
        return true;
    }
    return false;
  }
  bool legal(int move) {
    return (move >= 0 and move < 9 and this->current_board()[move] == player_mark::blank);
  }
public:
  const std::vector<game_board>& all_boards() const { return boards; }
  const std::vector<int>& all_moves() const { return moves; }
  const game_board& current_board() const { return boards.back(); }
  void play() {
    int move;
    while(!done) {
      game_board& board = boards.back();
      move = board.xturn() ? playerx.move(board) : playero.move(board);
      //std::cout << "play() move: " << move << std::endl;
      moves.push_back(move);
      if(legal(move)) {
        boards.push_back(board.next(move));
      }
      else {
        done = true;
      }
      game_board& nboard = boards.back();
      if(nboard.won()) {
        done = true;
      }
      if(nboard.filled()) {
        tie = true;
        done = true;
      }
    }
  }
  game_status result() const {
    if(tie) {
      return game_status::tie;
    }
    else {
      return boards.back().xturn() ? game_status::owins : game_status::xwins;
    }
  }
};