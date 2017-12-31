#include "tic_board.hpp"
#include "perfect_player.hpp"

#include <dll/network.hpp>
#include <dll/neural/dense_layer.hpp>

#include <vector>
#include <iterator>
#include <memory>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>

class ai_player_v2 {
  //static const int nclasses = 9;
  using network_t = dll::dyn_network_desc<
    dll::network_layers<
      dll::dense_layer<27, 81, dll::tanh>
      , dll::dense_layer<81, 27, dll::tanh>
      , dll::dense_layer<27, 1, dll::tanh>
    >
    , dll::batch_size<5000> 
    //, dll::updater<dll::updater_type::NADAM> 
   // , dll::shuffle
  >::network_t;
  std::vector<std::unique_ptr<network_t>> nets;
  static std::string data_fname() { return "ai_player_v2.data"; };
  static std::string net_fname() { return "ai_player_v2.net"; };
  perfect_player perfect;
  static std::string net_file(int i) {
    std::stringstream ss;
    ss << "ai_player_v2.net" << i;
    return ss.str();
  }
  /*void save_net() { 
    net->store(net_fname());
  }
  void load_net() {
    std::ifstream f(net_fname());
    if(f.good()) {
      f.close();
      net->load(net_fname());
    }
  }*/
  static std::vector<tic_board> get_boards(tic_board current = tic_board()) {
    std::vector<tic_board> boards{current};
    boards.push_back(current);
    for(int m=1; m<=9; ++m) {
      if(current.legal(m)) {
        auto next = current;
        next.move(m);
        if(!(next.won() or next.tie())) {
           auto next_boards = get_boards(next);
           boards.insert(boards.end(), next_boards.cbegin(), next_boards.cend());
        }
      }
    }
    return boards;
  }
  static auto to_data(const tic_board& board) {
    etl::fast_dyn_matrix<float, 27> data;
    std::fill(data.begin(), data.end(), 0);
    for(int i=0; i<9; ++i) {
      if(board[i] == board.x())
        data[3*i] = 1;
      else if(board[i] == board.o())
        data[3*i+1] = 1;
      else if(board[i] == board.blank())
        data[3*i+2] = 1;
    }
    return data;
  } 
public:
  ai_player_v2() {
    for(int i=0; i<9; ++i) {
      nets.push_back(std::make_unique<network_t>());
      nets[i]->load(net_file(i));
    }
  }
  void display() const {
    nets.front()->display();
  }
  int move(const tic_board& board) {
    auto data = to_data(board);
    std::array<float, 9> rs;
    for(int m=1; m<=9; ++m)
      rs[m-1] = nets[m-1]->features(data)[0];
    int m;
    if(board.player() == board.x())
      m = std::distance(rs.cbegin(), std::max_element(rs.cbegin(), rs.cend())) + 1;
    else
      m = std::distance(rs.cbegin(), std::min_element(rs.cbegin(), rs.cend())) + 1;
    return m;
  }
  void train() {
    std::vector<tic_board> boards = perfect.get_boards();
    //boards.resize(10);
    /*for(auto board : boards)
      std::cout << board << std::endl << std::endl;*/
    std::cout << boards.size() << std::endl;
    std::vector<etl::fast_dyn_matrix<float, 27>> data(boards.size());
    std::vector<std::vector<etl::fast_dyn_matrix<float, 1>>> rewards;
    rewards.resize(9);
    for(int i=0; i<boards.size(); ++i) {
      data[i] = to_data(boards[i]);
    }
    for(int m=1; m<=9; ++m) {
      rewards[m-1].resize(boards.size());
      for(int i=0; i<boards.size(); ++i)
        rewards[m-1][i][0] = perfect.reward(boards[i], m);
    }
    std::cout << "rewards[0].size(): " << rewards[0].size() << std::endl;
    auto accurate = [&](auto& net, auto& rewards){
      for(int i=0; i<boards.size(); ++i)
        if(round(net->features(data[i])[0]) != rewards[i][0])
          return false;
      return true;
    };
    auto accuracy = [&](auto& net, auto& rewards){
      int correct = 0;
      for(int i=0; i<boards.size(); ++i) {
        //std::cout << net->features(data[i])[0] << " -> " << round(net->features(data[i])[0]) << " == " << rewards[i][0] << std::endl;
        if(round(net->features(data[i])[0]) == rewards[i][0])
          ++correct;
      }
      return float(correct)/boards.size();
    };
    /*for(auto sample : data) {
      for(auto val : sample)
        std::cout << (val != 0 ? '1' : '0');
      std::cout << std::endl << std::endl;
    }*/
    //return;
    for(int i=0; i<9; ++i) {
      while(!accurate(nets[i], rewards[i])) {
        nets[i]->fine_tune_reg(data, rewards[i], 1);
        std::cout << "train accuracy[" << i << "]: " << accuracy(nets[i], rewards[i]) << std::endl;
        nets[i]->store(net_file(i));
      }
      std::cout << "train success: " << i+1 << "/9" << std::endl;
    }
  }
};