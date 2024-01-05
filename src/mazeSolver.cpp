#include "../include/mazeSolver.hpp"
#include "../include/constants.hpp"
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <tuple>
#include <utility>

using std::abs, std::queue, std::priority_queue, std::pair, std::map,
    std::tuple, std::make_tuple;

MazeSolver::MazeSolver(){};

int MazeSolver::getFrameCount() { return frames.size(); }

void MazeSolver::loadMazeFromFrame(string frame) {
  int i = 0;
  int j = 0;
  if (i >= field.size()) {
    field.emplace_back();
  }

  for (char ch : frame) {
    if (ch == '\n') {
      ++i;
      j = 0;

      if (i >= field.size()) {
        field.emplace_back();
      }

      continue;
    }

    if (j >= field[i].size()) {
      field[i].emplace_back();
    }

    field[i][j] = ch;
    ++j;
  }
}

void MazeSolver::addNewFrameFromField() {
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

string MazeSolver::getNextFrame() {
  if (currentStep >= frames.size())
    return frames[frames.size() - 1];
  return frames[currentStep++];
}

void MazeSolver::findStartFinish(int &si, int &sj, int &fi, int &fj) {
  for (int i = 0; i < field.size(); ++i)
    for (int j = 0; j < field[i].size(); ++j) {
      if (field[i][j] == START) {
        si = i;
        sj = j;
      }
      if (field[i][j] == FINISH) {
        fi = i;
        fj = j;
      }
    }
}

bool MazeSolver::isValidMove(int row, int col) {
  return row > 0 && row < field.size() - 1 && col > 0 &&
         col < field[0].size() - 1 &&
         (field[row][col] == EMPTY || field[row][col] == FINISH);
}

vector<pair<int, int>> MazeSolver::getNeighbors(pair<int, int> p) {
  vector<pair<int, int>> neighbors;
  vector<pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

  for (const auto &dir : directions) {
    int newRow = p.first + dir.first;
    int newCol = p.second + dir.second;

    if (isValidMove(newRow, newCol)) {
      neighbors.push_back({newRow, newCol});
    }
  }

  return neighbors;
}

void MazeSolver::solveMazeBFS() {
  int si, sj, fi, fj;
  findStartFinish(si, sj, fi, fj);
  pair<int, int> startCell = {si, sj};
  pair<int, int> endCell = {fi, fj};
  queue<pair<int, int>> q;
  map<pair<int, int>, pair<int, int>> visited;
  pair<int, int> currCell = startCell;
  visited.insert({currCell, {-1, -1}});
  addNewFrameFromField();

  while (currCell != endCell) {
    vector<pair<int, int>> neighbors = getNeighbors(currCell);

    for (const auto &cell : neighbors) {
      if (visited.find(cell) == visited.end()) {
        visited.insert({cell, currCell});
        if (cell != endCell)
          field[cell.first][cell.second] = VISITED;
        q.push(cell);
      }
    }

    currCell = q.front();
    q.pop();
    addNewFrameFromField();
  }

  vector<pair<int, int>> path;

  while (currCell != startCell) {
    auto it = visited.find(currCell);
    if (it != visited.end()) {
      currCell = it->second;
    }
    if (currCell != startCell) {
      path.insert(path.begin(), currCell);
    }
  }

  for (const auto &cell : path)
    field[cell.first][cell.second] = PATH;
  addNewFrameFromField();
}

int MazeSolver::getH(pair<int, int> cell1, pair<int, int> cell2) {
  return abs(cell1.first - cell2.first) + abs(cell1.second - cell2.second);
}

void MazeSolver::solveMazeAStar() {
  int si, sj, fi, fj;
  findStartFinish(si, sj, fi, fj);
  pair<int, int> startCell = {si, sj};
  pair<int, int> endCell = {fi, fj};

  vector<vector<int>> gScore(field.size(),
                             vector<int>(field[0].size(), 100000));
  gScore[startCell.first][startCell.second] = 0;
  vector<vector<int>> fScore(field.size(),
                             vector<int>(field[0].size(), 100000));
  fScore[startCell.first][startCell.second] = getH(startCell, endCell);

  priority_queue<tuple<int, int, pair<int, int>>,
                 vector<tuple<int, int, pair<int, int>>>, CompareCells>
      open;
  open.push(make_tuple(getH(startCell, endCell), getH(startCell, endCell),
                       startCell));

  vector<vector<pair<int, int>>> aPath(
      field.size(), vector<pair<int, int>>(field[0].size(), {0, 0}));

  while (open.size() != 0) {
    pair<int, int> currCell = get<2>(open.top());
    open.pop();

    if (currCell == endCell)
      break;

    if (currCell != startCell)
      field[currCell.first][currCell.second] = VISITED;
    addNewFrameFromField();

    vector<pair<int, int>> neighbors = getNeighbors(currCell);

    for (const auto &cell : neighbors) {
      int tempGScore = gScore[currCell.first][currCell.second] + 1;
      int tempFScore = tempGScore + getH(cell, endCell);

      if (tempFScore < fScore[cell.first][cell.second]) {
        gScore[cell.first][cell.second] = tempGScore;
        fScore[cell.first][cell.second] = tempFScore;
        open.push(make_tuple(tempFScore, getH(cell, endCell), cell));
        aPath[cell.first][cell.second] = currCell;
      }
    }
  }

  vector<pair<int, int>> path;

  pair<int, int> cell = endCell;
  while (cell != startCell) {
    path.push_back(cell);
    cell = aPath[cell.first][cell.second];
  }

  for (const auto &cell : path)
    if (cell != endCell)
      field[cell.first][cell.second] = PATH;
  addNewFrameFromField();
}
