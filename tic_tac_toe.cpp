#include <iostream>
#include <string>

//#include "tic_board.hpp"
#include "human_player.hpp"

template<class PlayerX, class PlayerO = PlayerO>
void play() {
  PlayerX playerx;
  PlayerO playero;
  while(true) {
    
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
