#include "game_session.hpp"
#include "human_player.hpp"

#inclue <iostream>
#include <string>

class interface {
public:
  void playtest(std::vector<std::string> players) {
   auto playloop = [](auto game){
    while(true) {
      game = Game();
      game.play();
      std::cout << game.winner() << " wins! " << std::endl;
      std::cout << "Play again? [y/n]: " <<;
      char ch;
      std::cin >> ch;
      if(ch != "y")
        break;
    }
   }
   if(players[0] == "human") {
     if(players[1] == "human") {
       playloop<tic_tac_toe<human_player, human_player>>();
     }
     else if(players[1] == "ai") {
    
     }
     else usage();
   }
   else if(players[0] == "ai") {
     if(players[1] == "human") {
        
     }
     else if(players[1] == "ai") {
         
     }
     else usage();
   }
   else usage();
  }
  void run(const std::vector<std::string>& args) {
    if(args.size() == 4) {
      if(args[1] == "playtest") {
        playtest({args[2], args[3]});
      }
    }
    else usage();
  }
  usage() {
    std::cout << argv[0] << " playtest [X] [O] (human or ai)" << std::endl
              << argv[0] << " train [batches] [games]" << std::endl;
  }
};

int main(int argc, char* argv[]) {
  std::vector<std::string> args(argc);
  for(int i=0; i<argc; ++i) 
    args[i] = argv[i];
  interface iface;
  iface.run(args);
  return 0;    
}
    
  return 0;
}