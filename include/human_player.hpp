#include "game_board.hpp"

#include <iostream>
#include <vector>

class human_player {
  void display(const game_board& board) {
    for(int r=0; r<3; ++r) {
      for(int c=0; c<3; ++c) {
        
      }
    }
  }
public:
  int move(const game_board& board) {
    display(board);
    std::cout << (board.xturn() ? "X" : "O") << " ";
    int m;
    std::cin >> m;
    return m - 1;
  }
};