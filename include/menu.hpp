#ifndef MENU_HPP
#define MENU_HPP

#include "fileManager.hpp"
#include <string>
#include <vector>

using std::string, std::vector;

class Menu {
private:
  vector<string> options = {"1. Generate maze", "2. Solve existing maze"};
  DisplayManager display;
  FileManager filesystem;

  void handleGenerateMaze();
  void handleSolveMaze();
  string getOptionsScreen();
  void showOptions();
  int getUserOption();
  void handleChosen(int option);

public:
  Menu(DisplayManager _display);

  bool run();
};

#endif
