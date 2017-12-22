#ifndef TIC_TAC_TOE_GAME_BOARD_HPP
#define TIC_TAC_TOE_GAME_BOARD_HPP

#include <bitset>
#include <iostream>

enum player_mark {x, o, blank};

struct game_board {
  const std::bitset<18> state;
  bool xturn() const { return state.count()%2 == 0; }
  bool filled() const { return state.count() == 9; }
  bool won() const {
    auto get_bit = [&](int r, int c) {
      return state[(r*3 + c)*2 + xturn()];
    };
    // rows
    for(int r=0; r<3; ++r) {
      if(get_bit(r, 0))
        if(get_bit(r, 1))
          if(get_bit(r, 2))
            return true;
    }
    // cols
    for(int c=0; c<3; ++c) {
      if(get_bit(0, c))
        if(get_bit(1, c))
          if(get_bit(2, c))
            return true;
    }
    // diag1
    if(get_bit(0, 0))
      if(get_bit(1, 1))
        if(get_bit(2, 2))
          return true;
    // diag2
    if(get_bit(0, 2))
      if(get_bit(1, 1))
        if(get_bit(2, 0))
          return true;
  }
  game_board next(int index) const {
    auto s = state;
    s.set((index*2)+!xturn());
    return { s };
  }
  player_mark operator[](int i) const {
    if(state[i*2])
      return player_mark::x;
    else if(state[i*2 + 1])
      return player_mark::o;
    else
      return player_mark::blank;
  }
  player_mark operator()(int r, int col) const {
    return (*this)[r*3 + col];
  }
  void display() const {
    for(int r=0; r<3; ++r) {
      for(int c=0; c<3; ++c) {
       switch((*this)(r, c)) {
         case player_mark::x:
           std::cout << "X";
           break;
         case player_mark::o:
           std::cout << "O";
           break;
         case player_mark::blank:
           std::cout << r*3 + c + 1;
       }
       std::cout << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
};

#endif // TIC_TAC_TOE_GAME_BOARD_HPP