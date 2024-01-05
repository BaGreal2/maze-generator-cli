#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP

#include <set>
#include <string>
#include <utility>
#include <vector>

using std::string, std::vector, std::pair, std::set;

struct Wall {
  pair<int, int> cell1;
  pair<int, int> cell2;
  Wall(pair<int, int> c1, pair<int, int> c2) : cell1(c1), cell2(c2){};
};

class MazeGenerator {
private:
  int currentStep = 0;
  vector<string> frames;
  vector<vector<char>> field;
  void addNewFrameFromField();
  vector<Wall> getWalls();
  void mergeSets(set<pair<int, int>> &set1, set<pair<int, int>> &set2);
  set<pair<int, int>> &
  findSetContainingCell(const pair<int, int> &cell,
                        vector<set<pair<int, int>>> &cellSets);

public:
  MazeGenerator();
  int getFrameCount();
  void initializeMaze(int rows, int cols);
  void generateMazeDFS(int row, int col);
  void generateMazeKruskal();
  void setStartFinish();
  string getNextFrame();
  string getStringField();
};

#endif
