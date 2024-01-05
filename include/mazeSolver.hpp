#ifndef MAZESOLVER_HPP
#define MAZESOLVER_HPP

#include <string>
#include <tuple>
#include <utility>
#include <vector>

using std::vector, std::string, std::pair, std::get;

struct CompareCells {
  bool operator()(const std::tuple<int, int, pair<int, int>> &a,
                  const std::tuple<int, int, pair<int, int>> &b) const {
    if (get<0>(a) > get<0>(b)) {
      return true;
    } else if (get<0>(a) < get<0>(b)) {
      return false;
    } else {
      return get<1>(a) > get<1>(b);
    }
  }
};

class MazeSolver {
private:
  int currentStep = 0;
  vector<vector<char>> field;
  vector<string> frames;
  void addNewFrameFromField();
  void findStartFinish(int &si, int &sj, int &fi, int &fj);
  bool isValidMove(int row, int col);
  int getH(pair<int, int> cell1, pair<int, int> cell2);
  vector<pair<int, int>> getNeighbors(pair<int, int> p);

public:
  MazeSolver();
  int getFrameCount();
  void loadMazeFromFrame(string frame);
  void solveMazeBFS();
  void solveMazeAStar();
  string getNextFrame();
};

#endif
