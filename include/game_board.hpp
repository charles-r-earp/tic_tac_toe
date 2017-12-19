#ifndef TIC_TAC_TOE_GAME_BOARD_HPP
#define TIC_TAC_TOE_GAME_BOARD_HPP

#include <bitset>

enum player_mark {x, o, blank};

struct game_board {
  const std::bitset<18> state;
  bool xturn() { return state.count()%2 == 0; }
  game_board next(int index) {
    return { state | (index*2)+xturn() };
  }
  player_mark operator[](int i) {
    if(state[i*2])
      return player_mark::x;
    else if(state[i*2 + 1])
      return player_mark::o;
    else
      return player_mark::blank;
  }
  player_mark operator()(int r, int col) {
    return *this[r*3 + col];
  } 
};

#endif // TIC_TAC_TOE_GAME_BOARD_HPP