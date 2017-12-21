#include "game_session.hpp"
#include "human_player.hpp"
#include "ai_player.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>

class interface {
public:
  template<class PlayerX, class PlayerO>
  void interactive_gameloop() {
    auto playagain = []{
     std::cout << "Play again? [y/n]: ";
     char ch;
     std::cin >> ch;
     std::cout << std::endl;
     return ch == 'y';
   };
    game_session<PlayerX, PlayerO> game;
    do {
      game = game_session<PlayerX, PlayerO>();
      game.play();
      game.current_board().display();
      switch(game.result()) {
        case game_status::tie:
          std::cout << "Tie!" << std::endl;
          break;
        case game_status::xwins:
          std::cout << "X wins!" << std::endl;
          break;
        case game_status::owins:
          std::cout << "O wins!" << std::endl;
      }
    } while(playagain());
  }
  bool playtest(std::vector<std::string> players) {
   if(players[0] == "human") {
     if(players[1] == "human") {
       interactive_gameloop<human_player, human_player>();
     }
     else if(players[1] == "ai") {
       interactive_gameloop<human_player, ai_player>();
     }
     else return false;
   }
   else if(players[0] == "ai") {
     if(players[1] == "human") {
       interactive_gameloop<ai_player, human_player>();
     }
     else return false;
   }
   else return false;
   return true;
  }
  void train(int games = 10) {
     ai_player ai;
    game_session<ai_player, ai_player> game;
    for(int i=0; i<games; ++i) {
      game.play();
      ai.train_game(game);
    }
    ai.train_epoch();
  }
  void run(const std::vector<std::string>& args) {
    if(args.size() == 3) {
      if(args[1] == "train") {
        int epochs = atoi(args[2].c_str());
        for(int i=0; i<epochs; ++i)
          train();
        return;
      } 
    }
    else if(args.size() == 4) {
      if(args[1] == "playtest") {
        bool success = playtest({args[2], args[3]});
        if(success) {
          return;
        }
      }
    }
    usage(args);
  }
  void usage(const std::vector<std::string>& args) {
    std::cout << args[0] << " playtest [X] [O] (human or ai)" << std::endl
              << args[0] << " train [batches] [games]" << std::endl;
  }
};

int main(int argc, char* argv[]) {
  ai_player ai;
  ai.display();
  std::vector<std::string> args(argc);
  for(int i=0; i<argc; ++i) 
    args[i] = argv[i];
  interface iface;
  iface.run(args);
  return 0;    
}