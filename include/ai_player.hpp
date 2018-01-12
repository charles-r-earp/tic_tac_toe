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

class ai_player {
  using network_t = dll::dyn_network_desc<
    dll::network_layers<
      dll::dense_layer<9, 9, dll::relu>
      , dll::dense_layer<9, 9, dll::relu>
	  , dll::dense_layer<9, 9, dll::relu>
	  , dll::dense_layer<9, 9, dll::identity>
    >
    , dll::batch_size<5000> 
    , dll::updater<dll::updater_type::NADAM> 
    , dll::shuffle
  >::network_t;	
  std::unique_ptr<network_t> net;
  std::string fname;
  void load() {
	std::ifstream f(fname);
	if(f.is_open()) {
	  f.close();
	  net->load(fname);
	}
  }
  static auto to_data(const tic_board& board) {
    etl::fast_dyn_matrix<float, 9> data;
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
  ai_player(std::string fname = "ai_player.net") {
	this->fname = fname;
	net = std::make_unique<network_t>();
    load();
  }
  int move(const tic_board& board) {
	auto data = to_data(board);
	auto vs =  net->features(data);
	std::cout << "evaluation: " << board.marks() << " -> ";
	for(auto v : vs)
      std::cout << v << " ";
	std::cout << std::endl;
	float v;
	if(board.player() == board.x())
	  v = *std::max_element(vs.cbegin(), vs.cend());
	else
	  v = *std::min_element(vs.cbegin(), vs.cend());
	std::array<int, 9> moves;
	for(int m=1; m<=9; ++m)
      moves[m-1] = m;
	static std::default_random_engine gen;
	std::shuffle(moves.begin(), moves.end(), gen);
	for(auto m : moves)
	  if(vs[m-1] == v)
	    return m;
	std::terminate();
  }
};