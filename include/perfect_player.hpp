#include "tic_board.hpp"

#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>

class perfect_player {
  std::map<std::string, int> results; 
  bool load(std::string fname) {
    //std::cout << "perfect_player loading: " << fname << std::endl;
    std::fstream f;
    f.open(fname, std::fstream::in);
    if(!f.is_open())
      return false;
    //std::cout << "perfect_player loaded." << std::endl;
    std::string line;
    std::stringstream ss;
    std::string key;
    int result;
    while(std::getline(f, line)) {
      ss << line;
      ss >> key;
      ss >> result;
      ss.clear();
      results[key] = result;
    }
    //std::cout << "results.empty() = " << results.empty() << std::endl;
    return !results.empty();
  } 
  std::string stringify(const tic_board& board, int m) const {
    std::stringstream ss;
    ss << board.marks() << "->" << m;
    return ss.str();
  }
  void build() {
    std::cout << "perfect_player building..." << std::endl;
    tic_board board;
    for(int m=1; m<=9; ++m)
      results[stringify(board, m)] = evaluate(board, m);
    //std::cout << "perfect_player build done." << std::endl;
  }
  int evaluate(tic_board board, int move) {
    auto player = board.player();
    if(!board.legal(move)) 
      return player == board.x() ? -1 : 1;
    board.move(move);  
    if(board.won())
      return player == board.x() ? 1 : -1;
    else if(board.tie())
      return 0;
    std::array<int, 9> rs;
    for(int m=1; m<=9; ++m)
      results[stringify(board, m)] = rs[m-1] = evaluate(board, m);
    if(player == board.x()) {
      if(std::find(rs.cbegin(), rs.cend(), -1) != rs.cend())
        return -1;
      if(std::find(rs.cbegin(), rs.cend(), 0) != rs.cend())
        return 0;
      return 1;
    }
    else {
      if(std::find(rs.cbegin(), rs.cend(), 1) != rs.cend())
        return 1;
      if(std::find(rs.cbegin(), rs.cend(), 0) != rs.cend())
        return 0;
      return -1;
    }
  }
  void save(std::string fname) const {
    std::fstream f(fname, std::fstream::out | std::fstream::trunc);
    for(auto entry : results)
      f << entry.first << " " << entry.second << std::endl;
  }
public:
  perfect_player(std::string fname = "perfect_player.data") {
    if(!load(fname)) {
      build();
      save(fname);
    }
  }
  int move(const tic_board& board) const {
    std::array<int, 9> rs;
    for(int m=1; m<=9; ++m)
      rs[m-1] = results.at(stringify(board, m));
    int m;
    if(board.player() == board.x())
      m = std::distance(rs.cbegin(), std::max_element(rs.cbegin(), rs.cend())) + 1;
    else
      m = std::distance(rs.cbegin(), std::min_element(rs.cbegin(), rs.cend())) + 1;
    //std::cout << stringify(board, m) << " " << rs[m-1] << std::endl;
    return m;
  }
};