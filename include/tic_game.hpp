#include "tic_board.hpp"

struct tic_game {
  std::vector<tic_board> _boards;
  std::vector<int> _moves;
  template<class PlayerX, class PlayerO = PlayerX>
  static tic_game play() {
    PlayerX playerx;
    PlayerO playero;
    while(true) {
    }
  }
};



