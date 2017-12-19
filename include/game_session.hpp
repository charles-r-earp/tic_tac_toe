#incclude <vector>

enum game_status {tie, xwins, owins};

template<class PlayerX, class PlayerO>
class game_session {
  PlayerX playerx;
  PlayerO playero;
  std::vector<game_board> boards(1);
  bool done = false, tie = false;
  bool game_won() {
    return false;
  }
  bool legal(int move) {
    return move >= 0 and move < 9;
  }
public:
  void play() {
    int move;
    game_board board;
    while(!done) {
      board = boards.back();
      move = board.xturn() ? playerx.move(board) : playero.move(board);
      if(legal(move)) {
        boards.push_back(board.next(move));
      }
      else {
        done = true;
      }
      if(game_won()) {
        done = true;
      }
      if(boards.size() == 9) {
        tie = true;
        done = true;
      }
    }
  }
  game_status result() {
    if(tie) {
      return game_status::tie;
    }
    else {
      return xturn() ? game_status::owins : gamestatus::xwins;
    }
  }
};