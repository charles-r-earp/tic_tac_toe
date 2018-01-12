#ifndef TIC_TAC_TOE_PERFECT_PLAYER_HPP
#define TIC_TAC_TOE_PERFECT_PLAYER_HPP

#include "tic_board.hpp"

#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <string>
#include <iostream>

class perfect_player {
  std::map<tic_board, std::array<float, 9>> _qvalues;
  bool load(std::string fname) {
    //std::cout << "perfect_player loading: " << fname << std::endl;
    std::fstream f;
    f.open(fname, std::fstream::in);
    if(!f.is_open())
      return false;
    //std::cout << "perfect_player loaded." << std::endl;
    std::string line;
    std::stringstream ss;
    std::string marks;
    while(std::getline(f, line)) {
      ss << line;
      ss >> marks;
	  for(auto& v : _qvalues[tic_board(marks)])
	    ss >> v;
    }
    //std::cout << "results.empty() = " << results.empty() << std::endl;
    return !_qvalues.empty();
  } 
  /*std::string stringify(const tic_board& board, int m) const {
    std::stringstream ss;
    ss << board.marks() << "->" << m;
    return ss.str();
  }*/
  void build() {
    //std::cout << "perfect_player building..." << std::endl;
	alpha_beta(tic_board(), -1, 1);
    /*tic_board board;
    for(int m=1; m<=9; ++m)
      results[stringify(board, m)] = evaluate(board, m);*/
    //std::cout << "perfect_player build done." << std::endl;
  }
  float alpha_beta(tic_board board, float a, float b) {
	 if(board.won()) {
	   return board.player() == board.o() ? 1 : -1;
	 }
	 else if(board.tie()) {
	   return 0;
	 }
	 float v, q;
	 float df = 0.9f; // discount factor
	 if(board.player() == board.x()) {
	   v = -1;
	   for(int m=1; m<=9; ++m) {
		 if(board.legal(m))
	       q = alpha_beta(board.next(m), a, b);
		 else
		   q = -1;
		 _qvalues[board][m-1] = q;
		 v = std::max(v, q);
		 a = std::max(a, v); 
		 /*if(b <= a)
			break;*/
	   }
	 }
	 else {
	   v = 1;
	   for(int m=1; m<=9; ++m) {
		 if(board.legal(m))
	       q = alpha_beta(board.next(m), a, b);
		 else
		   q = 1;
		 _qvalues[board][m-1] = q;
		 v = std::min(v, q);
		 b = std::min(b, v);
		 /*if(b <= a)
			break;*/
	   }
	 }
	 return v*df;
  }
  /*int evaluate(tic_board board, int move) {
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
  }*/
  void save(std::string fname) const {
    std::fstream f(fname, std::fstream::out | std::fstream::trunc);
    for(auto& entry : _qvalues) {
      f << entry.first.marks() << " ";
	  for(auto& v : entry.second)
		  f << v << " ";
	  f << std::endl;
	}
  }
public:
  perfect_player(std::string fname = "perfect_player.data") {
    //if(!load(fname)) {
      build();
      save(fname);
    //}
    //std::cout << "perfect_player results.size() " << results.size() << std::endl;
	//alpha_beta(tic_board(), -1, 1);
	std::cout << "perfect_player qvalues.size() " << _qvalues.size() << std::endl;
  }
  const auto& qvalues() const { return _qvalues; }
  /*std::vector<tic_board> get_boards() const {
    std::vector<tic_board> boards;
    std::map<std::string, bool> visited;
    std::string marks;
    for(auto entry : results) {
      marks = entry.first.substr(0, 9);
      if(visited.find(marks) == visited.end()) {
        visited[marks] = true;
        boards.push_back(tic_board(marks));
      }
    }
    return boards;
  }*/
  /*int reward(const tic_board& board, int m) {
    //std::cout << "reward(" << stringify(board, m) << ")" << std::endl;
    return qvalues.at(stringify(board, m));
  }*/
  int move(const tic_board& board) {
	auto it = _qvalues.find(board);
	if(it == _qvalues.cend())
	  alpha_beta(board, -1, 1);
	auto vs = _qvalues.at(board);
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
    /*std::vector<int> mvs;
	std::vector<float> vs;
    for(int m=1; m<=9; ++m) {
	  auto it = qvalues.find({board, m});
	  if(it != qvalues.cend()) {
		mvs.push_back(m);
		vs.push_back(it->second);
	  }
	}
	std::cout << "evaluation: " << std::endl;
	std::cout << board.marks() << std::endl;
	for(int m=1; m<=9; ++m)
	  std::cout << m << " -> " << vs[m-1] << std::endl;
	float best;
	if(board.player() == board.x())
	  best = *std::max_element(vs.cbegin(), vs.cend());
	else
	  best = *std::min_element(vs.cbegin(), vs.cend());
	std::vector<int> best_mvs;
	for(int i=0; i<mvs.size(); ++i) {
	  if(vs[i] == best) {
	    best_mvs.push_back(mvs[i]);
	  }
	}
	static std::default_random_engine gen;
	std::uniform_int_distribution<int> dist(0, best_mvs.size());
	return best_mvs[dist(gen)];*/
  }
};

#endif // TIC_TAC_TOE_PERFECT_PLAYER_HPP