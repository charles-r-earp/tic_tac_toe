#include <string>
#include <cassert>
#include <ostream>



class tic_board {
  std::string _marks;
public:
  static const signed char x = 'X', o = 'O', blank = '_';
  tic_board(std::string marks = "_________") : _marks(marks) {
    assert(marks.length() == 9);
    for(auto m : marks)
      assert(m == x or m == o or m == blank);
  }
  const std::string& marks() const { return _marks; }
  friend std::ostream& operator<<(std::ostream& os, const tic_board& board) {
    int i = 0;
    for(const auto m : board.marks()) {
      os << (m == blank ? i+1 : m);
      if((i+1) % 3) 
        os << ' ';
      else if(i < 8)
        os << '\n';
      ++i;
    }
    return os;
  }
};
