#include "game_board.hpp"

#include <dll/network.hpp>
#include <dll/neural/dense_layer.hpp>

#include <vector>
#include <iterator>
#include <memory>
#include <fstream>
#include <algorithm>
#include <random>
#include <sstream>
#include <cassert>
#include <cmath>

class ai_player {
  static const int nclasses = 9;
  using network_t = dll::dyn_network_desc<
    dll::network_layers<
      dll::dense_layer<36, 36, dll::tanh>
      , dll::dense_layer<36, 9, dll::softmax>
    >
  >::network_t;
  std::unique_ptr<network_t> net = std::make_unique<network_t>();
  static std::string data_fname() { return "ai_player.data"; };
  static std::string net_fname() { return "ai_player.net"; };
  std::default_random_engine generator;
  std::vector<std::string> lines;
  void save_net() { 
    net->store(net_fname());
  }
  void load_net() {
    std::ifstream f(net_fname());
    if(f.good()) {
      f.close();
      net->load(net_fname());
    }
  }
public:
  void display() const {
    net->display();
  }
  int move(const game_board& board) {
    etl::fast_dyn_matrix<float, 36> data;
    for(int u=0; u<18; ++u) {
      data[u] = board.state[u];
    }
    std::vector<float> rewards(9);
    int move_index;
    for(int i=0; i<9; ++i) {
      move_index = i*2 + !board.xturn();
      data[move_index] = 1;
      rewards[i] = float(2* net->predict(data)) / (nclasses - 1);
      if(!board.xturn())
        rewards[i] = -rewards[i];
      data[move_index] = 0;
    }
    auto it = std::max_element(rewards.cbegin(), rewards.cend());
    int index = std::distance(rewards.cbegin(), it);
    //std::cout << "index: " << index << std::endl;
    return index;
  }
  template<class Game>
  void train_game(const Game& game) {
    //std::cout << "train_game() " << std::endl;
    std::fstream data_fstream(data_fname(), std::fstream::in | std::fstream::app);
    auto move_it = game.all_moves().cbegin();
    auto move_end = game.all_moves().cend();
    auto board_it = game.all_boards().cbegin();
    auto board_end = game.all_boards().cend();
    float result;
    if(game.result() == game_status::xwins) {
      result = 1;  
    }
    else if(game.result() == game_status::owins) {
      result = -1;
    }
    std::stringstream ss;
    for(; move_it != move_end; ++move_it, ++board_it) {
      //std::cout << "write move: " << *move_it << std::endl;
      ss << board_it->state << '\t' << *move_it << '\t' << result/std::distance(move_it, move_end);
      data_fstream << ss.str() << std::endl;
      lines.push_back(ss.str());
      ss.clear();
    }
    //std::cout << "train_game() done." << std::endl;
  }
  void train_epoch(int batch_size = 1000) {
    //std::cout << "train_epoch()" << std::endl;
    if(lines.empty()) {
      std::ifstream data_fstream(data_fname());
      std::string line;
      while(std::getline(data_fstream, line)) {
        lines.push_back(line);
      }
    }
    std::uniform_int_distribution<int> distribution(0, lines.size());
    load_net();
    std::vector<etl::fast_dyn_matrix<float, 36>> data(batch_size);
    std::vector<int> labels(batch_size);
    std::stringstream ss;
    std::string line;
    int n = distribution(generator);
    std::bitset<18> state;
    int move;
    float reward;
    for(int i=0; i<batch_size; ++i) {
      line = lines[n];
      //std::cout << "line: " << line << std::endl;
      ss << line;
      ss >> state;
      ss >> move;
      ss >> reward;
      ss.clear();
      for(int u=0; u<18; ++u)
        data[i][u] = state[u];
      //std::cout << "move: " << move << std::endl;
      if(state.count()%2)
        data[i][17+move*2] = 1;
      else
        data[i][17+move*2 + 1] = 1;
      labels[i] = round(((reward + 1) / 2) * (nclasses-1));
      //std::cout << "label: " << labels[i] << std::endl;
    };
    net->fine_tune(data, labels, 1);
    save_net();
    //std::cout << "train_epoch() done." << std::endl;
  }
};