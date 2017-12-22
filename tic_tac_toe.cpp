#include <iostream>
#include <string>

#include "tic_game.hpp"
#include "human_player.hpp"
#include "perfect_player.hpp"

void play() {
  while(true) {
    tic_game game = tic_game::play<human_player, perfect_player>();
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
}

void usage(const std::string& name) {
  std::cout << name << " version 0.0" << std::endl
            << "usage: " << std::endl
            << name << std::endl
            << name << " -train" << std::endl;  
}

int main(int argc, char* argv[]) {
  if(argc == 1)
    play();
  else if(argc == 2)
    if(argv[2] == "-train") 
      train();
    else
      usage(argv[0]);
  else
    usage(argv[0]);
  return 0;    
}
