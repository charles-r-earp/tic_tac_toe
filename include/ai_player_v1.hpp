#include "game_board.hpp"
#include "perfect_player.hpp"

#include <dll/network.hpp>
#include <dll/neural/dense_layer.hpp>

#include <vector>
#include <iterator>
#include <memory>
#include <fstream>
#include <algorithm>
#include <random>
#include <sstream>

class ai_player_v1 {
  static const int nclasses = 9;
  using network_t = dll::dyn_network_desc<
    dll::network_layers<
      dll::dense_layer<18, 3, dll::softmax>
    >
  >::network_t;
  std::unique_ptr<network_t> nets = std::make_unique(new network_t[9]);
  static std::string data_fname() { return "ai_player_v1.data"; };
  static std::string net_fname() { return "ai_player_v1.net"; };
  perfect_player perfect;
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
    nets->display();
  }
  int move(const game_board& board) {
    etl::fast_dyn_matrix<float, 18> data;
    for(int u=0; u<18; ++u) {
      data[u] = board.state[u];
    }
    return 1;
  }
};