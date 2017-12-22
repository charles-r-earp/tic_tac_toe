#include "tic_board.hpp"

#include <vector>

struct tic_game {
  std::vector<tic_board> boards;
  std::vector<int> moves;
  int result;
  template<class PlayerX, class PlayerO = PlayerX>
  static tic_game play() {
    PlayerX playerx;
    PlayerO playero;
    tic_game game;
    int m;
    tic_board board;
    game.boards.push_back(board);
    while(true) {
      m = playerx.move(board);
      game.moves.push_back(m);
      if(!board.legal(m)) {
        game.result = -1;
        break;
      }
      board.move(m);
      game.boards.push_back(board);
      if(board.won()) {
        game.result = 1;
        break;
      }
      if(board.tie()) {
        game.result = 0;
        break;
      }
      m = playero.move(board);
      game.moves.push_back(m);
      if(!board.legal(m)) {
        game.result = 1;
        break;
      }
      board.move(m);
      game.boards.push_back(board);
      if(board.won()) {
        game.result = -1;
        break;
      }
      if(board.tie()) {
        game.result = 0;
        break;
      }
    }
    return game;
  }
};


