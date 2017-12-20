#include "tic_board.hpp"

#include <iostream>
#include <vector>

class human_player {
public:
  int move(const game_board& board) {
    std::cout << board << std::endl << std::endl; 
    std::cout << board.player() << ": ";
    int m;
    std::cin >> m;
    return m;
  }
};
