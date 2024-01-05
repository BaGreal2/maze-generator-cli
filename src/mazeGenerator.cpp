#include "../include/mazeGenerator.hpp"
#include "../include/constants.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <utility>

using std::pair, std::shuffle, std::random_device, std::set;

MazeGenerator::MazeGenerator(){};

void MazeGenerator::addNewFrameFromField() {
  string newFrame;
  for (auto line : field) {
    for (auto ch : line) {
      newFrame += ch;
      newFrame += EMPTY;
    }
    newFrame += '\n';
  }

  frames.push_back(newFrame);
};

void MazeGenerator::initializeMaze(int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    vector<char> temp;
    for (int j = 0; j < cols; ++j) {
      temp.push_back(BORDER);
    }
    if (rows % 2 == 0)
      temp.push_back(BORDER);
    field.push_back(temp);
  }
  vector<char> temp;
  if (rows % 2 == 0)
    for (int j = 0; j <= cols; ++j) {
      temp.push_back(BORDER);
    }
  field.push_back(temp);
}

void MazeGenerator::generateMazeDFS(int row, int col) {
  field[row][col] = EMPTY;

  vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  random_device rd;
  std::mt19937 g(rd());
  shuffle(directions.begin(), directions.end(), g);

  for (const auto &dir : directions) {
    int newRow = row + dir.first * 2;
    int newCol = col + dir.second * 2;

    if (newRow > 0 && newRow < field.size() - 1 && newCol > 0 &&
        newCol < field[0].size() - 1 && field[newRow][newCol] == BORDER) {
      field[row + dir.first][col + dir.second] = EMPTY;

      MazeGenerator::addNewFrameFromField();
      MazeGenerator::generateMazeDFS(newRow, newCol);
    }
  }
}

string MazeGenerator::getNextFrame() {
  if (currentStep >= frames.size())
    return frames[frames.size() - 1];
  return frames[currentStep++];
}

int MazeGenerator::getFrameCount() { return frames.size(); }

string MazeGenerator::getStringField() {
  string strField;
  for (auto line : field) {
    for (auto ch : line) {
      strField += ch;
    }
    strField += '\n';
  }
  return strField;
}

void MazeGenerator::setStartFinish() {
  field[1][1] = START;
  int i = field.size() - 2;
  int j = field[0].size() - 2;
  bool rightDone = false;
  while (field[i][j] == BORDER) {
    if (i == 1) {
      rightDone = true;
      i = field.size() - 1;
      continue;
    }

    if (rightDone) {
      --j;
    } else {
      --i;
    }
  }
  field[i][j] = FINISH;
  addNewFrameFromField();
}

void MazeGenerator::mergeSets(set<pair<int, int>> &set1,
                              set<pair<int, int>> &set2) {
  for (const auto &element : set2) {
    set1.insert(element);
  }
  set2.clear();
}

set<pair<int, int>> &
MazeGenerator::findSetContainingCell(const pair<int, int> &cell,
                                     vector<set<pair<int, int>>> &cellSets) {
  for (set<pair<int, int>> &cellSet : cellSets) {
    auto it = cellSet.find(cell);
    if (it != cellSet.end()) {
      return cellSet;
    }
  }
  static set<pair<int, int>> emptySet;
  return emptySet;
}

vector<Wall> MazeGenerator::getWalls() {
  vector<Wall> walls;
  for (int i = 1; i < field.size(); i += 2) {
    for (int j = 1; j < field[i].size(); j += 2) {
      field[i][j] = EMPTY;
      if (i - 1 != 0)
        walls.push_back({{i, j}, {i - 2, j}});
      if (j - 1 != 0)
        walls.push_back({{i, j}, {i, j - 2}});
      if (i + 1 != field.size() - 1)
        walls.push_back({{i, j}, {i + 2, j}});
      if (j + 1 != field[i].size() - 1)
        walls.push_back({{i, j}, {i, j + 2}});
    }
  }
  addNewFrameFromField();
  return walls;
}

void MazeGenerator::generateMazeKruskal() {
  vector<Wall> walls = getWalls();
  random_device rd;
  std::mt19937 g(rd());
  shuffle(walls.begin(), walls.end(), g);

  vector<set<pair<int, int>>> cellSets;

  for (int i = 1; i < field.size(); i += 2) {
    for (int j = 1; j < field[i].size(); j += 2) {
      set<pair<int, int>> cellSet;
      cellSet.insert({i, j});
      cellSets.push_back(cellSet);
    }
  }

  for (const Wall &wall : walls) {
    set<pair<int, int>> &firstSet = findSetContainingCell(wall.cell1, cellSets);
    set<pair<int, int>> &secondSet =
        findSetContainingCell(wall.cell2, cellSets);

    if (firstSet != secondSet) {
      mergeSets(firstSet, secondSet);
      field[(wall.cell1.first + wall.cell2.first) / 2]
           [(wall.cell1.second + wall.cell2.second) / 2] = EMPTY;

      addNewFrameFromField();
    }
  }
}
