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

class ai_player_v5 {
  static const int nhidden = 18;
  using network_t = dll::dyn_network_desc<
    dll::network_layers<
      dll::dense_layer<9, nhidden, dll::relu>
      , dll::dense_layer<nhidden, nhidden, dll::relu>
	  , dll::dense_layer<nhidden, 9, dll::tanh>
    >
    , dll::batch_size<5000> 
    //, dll::updater<dll::updater_type::NADAM> 
   // , dll::shuffle
  >::network_t;
  std::unique_ptr<network_t> net;
  //static std::string data_fname() { return "ai_player_v9.data"; };
  //static std::string net_fname() { return "ai_player_v9.net"; };
  perfect_player perfect;
  std::ofstream log = std::ofstream(log_file(), std::ofstream::trunc);
  static std::string net_file() {
    std::stringstream ss;
    ss << "ai_player_v5.net";
    return ss.str();
  }
  static std::string log_file() {
    std::stringstream ss;
    ss << "ai_player_v5.log";
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
    etl::fast_dyn_matrix<float, 9> data;
    //std::fill(data.begin(), data.end(), 0);
    for(int i=0; i<9; ++i) {
      if(board[i] == board.x())
        data[i] = 1;
      else if(board[i] == board.o())
        data[i] = -1;
      else if(board[i] == board.blank())
        data[i] = 0;
    }
    return data;
  } 
public:
  ai_player_v5() {
    net = std::make_unique<network_t>();
	net->log = &log;
    net->load(net_file());
  }
  void display() const {
    net->display();
  }
  int move(const tic_board& board) {
    auto data = to_data(board);
    return net->predict(data) + 1;
  }
  void train() {
    std::vector<tic_board> boards = perfect.get_boards();
	boards.resize(10);
    std::cout << boards.size() << std::endl;
    std::vector<etl::fast_dyn_matrix<float, 9>> data(boards.size());
    std::vector<etl::fast_dyn_matrix<float, 9>> rewards(boards.size());
	std::vector<int> labels(boards.size()); 
    for(int i=0; i<boards.size(); ++i) {
      data[i] = to_data(boards[i]);
	  labels[i] = perfect.move(boards[i]) - 1;
	  for(int u=0; u<9; ++u)
	    rewards[i][u] = perfect.reward(boards[i], u+1);
    }
    /*auto accurate = [&](auto& net, auto& rewards){
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
    };*/
    /*for(auto sample : data) {
      for(auto val : sample)
        std::cout << (val != 0 ? '1' : '0');
      std::cout << std::endl << std::endl;
    }*/
    //return;
	float error = 1;
    while(error) {
      error = net->fine_tune_reg(data, rewards, 1);
	  std::cout << "error: " << error << std::endl;
	  //error = net->evaluate_error(data, labels);
      //std::cout << "train accuracy: " << 1-error << "\r" << std::flush;
      net->store(net_file());
    }
    std::cout << "\ntrain success" << std::endl;
  }
};