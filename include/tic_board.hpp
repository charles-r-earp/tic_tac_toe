#ifndef TIC_TAC_TOE_TIC_BOARD_HPP
#define TIC_TAC_TOE_TIC_BOARD_HPP

#include <string>
#include <cassert>
#include <ostream>
#include <algorithm>

class tic_board {
  std::string _marks;
public:
  static signed char x() { return 'X'; }
  static signed char o() { return 'O'; }
  static signed char blank() { return '_'; }
  tic_board(std::string marks = std::string(9, blank())) : _marks(marks) {
    assert(marks.length() == 9);
    for(auto m : marks)
      assert(m == x() or m == o() or m == blank());
  }
  auto player() const {
    int xs = std::count(_marks.cbegin(), _marks.cend(), x());
    int os = std::count(_marks.cbegin(), _marks.cend(), o());
    return xs <= os ? x() : o();
  }
  const auto& operator[](const int i) const {
    return _marks[i];
  } 
  void move(int m) {
    _marks[m-1] = player();
  }
  tic_board next(int m) {
	 tic_board board = *this;
	 board.move(m);
	 return board;
  }
  bool legal(int m) const {
    return _marks[m-1] == blank();
  }
  bool tie() {
    int bs = std::count(_marks.cbegin(), _marks.cend(), blank());
    return bs == 0;
  }
  bool won() {
    auto mark = (player() == x()) ? o() : x();
    for(int r=0; r<3; ++r)
      if(_marks[3*r] == mark and _marks[3*r+1] == mark and _marks[3*r+2] == mark)
        return true;
    for(int c=0; c<3; ++c)
      if(_marks[c] == mark and _marks[3+c] == mark and _marks[6+c] == mark)
        return true;
    if(_marks[0] == mark and _marks[4] == mark and _marks[8] == mark)
      return true;
    if(_marks[6] == mark and _marks[4] == mark and _marks[2] == mark)
      return true;
    return false;
  }
  const std::string& marks() const { return _marks; }
  bool operator<(const tic_board& rhs) const { return _marks < rhs._marks; }
  friend std::ostream& operator<<(std::ostream& os, const tic_board& board) {
    int i = 0;
    for(const auto m : board.marks()) {
      if(m == blank())
        os << i+1; 
      else 
        os << m;
      if((i+1) % 3) 
        os << ' ';
      else if(i < 8)
        os << '\n';
      ++i;
    }
    return os;
  }
};

#endif // TIC_TAC_TOE_TIC_BOARD_HPP