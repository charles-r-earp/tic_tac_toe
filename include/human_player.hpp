#include "game_board.hpp"

#include <iostream>
#include <vector>

class human_player {
public:
  int move(const game_board& board) {
    board.display();
    std::cout << (board.xturn() ? "X" : "O") << ": ";
    int m;
    std::cin >> m;
    std::cout << "------" << std::endl << std::endl;
    return m - 1;
  }
};