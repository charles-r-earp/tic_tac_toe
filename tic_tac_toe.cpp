#include "tic_game.hpp"
#include "human_player.hpp"
#include "perfect_player.hpp"
#include "ai_player_v6.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

void play() {
  while(true) {
    tic_game game = tic_game::play<human_player, ai_player_v6>();
    std::cout << game.boards.back() << std::endl << std::endl;
    if(game.result == 1) 
      std::cout << "X wins!";
    else if(game.result == -1)
      std::cout << "O wins!";
    else
      std::cout << "Tie!";
    std::cout << std::endl << "Play again? ";
    char c; 
    std::cin >> c;
    if(c != 'y')
      break;
    std::cout << std::endl;
  }
}

void train() {
  ai_player_v6 ai;
  ai.train();
}

void test() {
  int ai_wins = 0, ties = 0, perfect_wins = 0;
  for(int i=0; i<50; ++i) {
    tic_game game = tic_game::play<ai_player_v6, perfect_player>();
    if(game.result == 1)
      ++ai_wins;
    else if(game.result == -1)
      ++perfect_wins;
    else
      ++ties;
  }
  for(int i=0; i<50; ++i) {
    tic_game game = tic_game::play<perfect_player, ai_player_v6>();
    if(game.result == 1)
      ++ai_wins;
    else if(game.result == -1)
      ++perfect_wins;
    else
      ++ties;
  }
  std::cout << "test 100 games" << std::endl
            << "ai: " << ai_wins << std::endl
            << "perfect: " << perfect_wins << std::endl
            << "draw: " << ties << std::endl;
}

void usage(const std::string& name) {
  std::cout << name << " version 1.1" << std::endl
            << "usage: " << std::endl
            << name << std::endl
            << name << " -train" << std::endl  
            << name << " -test" << std::endl;  
}

int main(int argc, char* argv[]) {
  if(argc == 1)
    play();
  else if(argc == 2) {
    if(std::string(argv[1]) == "-train") {
      train();
    }
    else if(std::string(argv[1]) == "-test")
      test();
    else {
      usage(argv[0]);
    }
  }
  else
    usage(argv[0]);
  return 0;    
}
