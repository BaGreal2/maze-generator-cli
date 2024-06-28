#include "../include/menu.hpp"
#include "../include/mazeGenerator.hpp"
#include "../include/mazeSolver.hpp"
#include <chrono>
#include <string>

using std::chrono::high_resolution_clock, std::chrono::duration_cast,
    std::chrono::milliseconds, std::to_string;

Menu::Menu(DisplayManager _display, FileManager _filesystem)
    : filesystem(_filesystem), display(_display){};

string Menu::getOptionsScreen() {
  string result;
  for (int i = 0; i < options.size(); ++i) {
    result += options[i];
    result += '\n';
  }
  return result;
}

void Menu::handleChosen(int option) {
  switch (option) {
  case 1:
    handleGenerateMaze();
    break;
  case 2:
    handleSolveMaze();
    break;
  case 0:
    break;
  default:
    run();
    break;
  }
}

void Menu::showOptions() {
  display.clearScreen();
  display.displayFrame("Choose an option:\n", "green");
  display.displayFrame(getOptionsScreen());
  display.displayFrame("0. Exit\n", "red");
}

int Menu::getUserOption() {
  display.displayFrame("Your option -> ");
  int option = display.readInput<int>();
  return option;
}

bool Menu::run() {
  showOptions();
  int option = getUserOption();
  if (option == 0) {
    return false;
  }
  handleChosen(option);
  return true;
}

void Menu::handleGenerateMaze() {
  display.clearScreen();
  int size = -1;
  while (true) {
    display.clearScreen();
    display.displayFrame("Insert the size of a maze ");
    display.displayFrame("(0 to return to menu)", "yellow");
    display.displayFrame(" -> ");
    size = display.readInput<int>();
    if (size == 0) {
      return;
    } else if (size < 5) {
      display.displayFrame("\nToo small size!\n", "red");
      display.sleep(700);
      continue;
    } else if (size > 80) {
      display.displayFrame("\nToo big size!\n", "red");
      display.sleep(700);
      continue;
    } else {
      break;
    }
  }
  if (size % 2 != 0) {
    ++size;
  }
  MazeGenerator mazeGen;
  mazeGen.initializeMaze(size, size);

  while (true) {
    display.clearScreen();
    display.displayFrame("Choose an algorithm:", "green");
    display.displayFrame("\n1. DFS\n2. Kruskal\n");
    display.displayFrame("0. Return to main\n", "yellow");
    display.displayFrame("\nYour option -> ");
    int algorithm = display.readInput<int>();
    if (algorithm == 0) {
      return;
    } else if (algorithm == 1) {
      mazeGen.generateMazeDFS(1, 1);
      break;
    } else if (algorithm == 2) {
      mazeGen.generateMazeKruskal();
      break;
    } else {
      display.displayFrame("\nInvalid option!\n", "red");
      display.sleep(1300);
      continue;
    }
  }
  for (int i = 0; i < mazeGen.getFrameCount(); ++i) {
    display.displayFrame(mazeGen.getNextFrame(), "maze");
    display.sleep(20);
    display.clearScreen();
  }
  mazeGen.setStartFinish();
  string maze = mazeGen.getNextFrame();
  string strMaze = mazeGen.getStringField();

  bool running = true;
  while (true) {
    display.clearScreen();
    display.displayFrame(maze, "maze");
    display.displayFrame("\nWould you like to save maze to a file (y/n) -> ");
    char saveToFile = display.readInput<char>();
    if (saveToFile == 'y') {
      display.displayFrame("Name of the file (without spaces) -> ");
      string fileName = display.readInput<string>();
      int code = filesystem.save(fileName, strMaze);
      if (code == 0) {
        display.displayFrame("\nFile saved successfully!\n", "green");
        display.sleep(1300);
        return;
      } else {
        display.displayFrame("\nError saving a file!\n", "red");
        display.sleep(1300);
      }
    } else {
      return;
    }
  }
}

void Menu::handleSolveMaze() {
  int fileIndex = -1;
  vector<string> names = filesystem.getSavedFilesNames();

  while (true) {
    names = filesystem.getSavedFilesNames();
    display.clearScreen();
    display.displayFrame("Choose a file to load:\n", "green");
    int i = 1;
    for (const auto &name : names) {
      if (name == ".DS_Store")
        continue;
      display.displayFrame(to_string(i) + ". " + name + "\n");
      ++i;
    }
    if (i == 1)
      display.displayFrame("No files.\n\n", "red");

    display.displayFrame("0. Return to main menu\n", "yellow");
    display.displayFrame("\nProvide the number of a file -> ");
    fileIndex = display.readInput<int>();
    if (fileIndex == 0) {
      return;
    } else if (fileIndex < 0 || fileIndex > names.size()) {
      display.displayFrame("\nInvalid option!\n", "red");
      display.sleep(1300);
      continue;
    } else {
      break;
    }
  }

  string maze = filesystem.read(names[fileIndex - 1]);
  MazeSolver mazeSolve;
  mazeSolve.loadMazeFromFrame(maze);
  int duration = 0;

  while (true) {
    display.clearScreen();
    display.displayFrame("Choose an algorithm:", "green");
    display.displayFrame("\n1. BFS\n2. A*\n");
    display.displayFrame("0. Return to main\n", "yellow");
    display.displayFrame("\nYour option -> ");
    int algorithm = display.readInput<int>();
    if (algorithm == 0) {
      return;
    } else if (algorithm == 1) {
      auto start = high_resolution_clock::now();
      mazeSolve.solveMazeBFS();
      auto stop = high_resolution_clock::now();
      duration = duration_cast<milliseconds>(stop - start).count();
      break;
    } else if (algorithm == 2) {
      auto start = high_resolution_clock::now();
      mazeSolve.solveMazeAStar();
      auto stop = high_resolution_clock::now();
      duration = duration_cast<milliseconds>(stop - start).count();
      break;
    } else {
      display.displayFrame("\nInvalid option!\n", "red");
      display.sleep(1300);
      continue;
    }
  }

  for (int i = 0; i < mazeSolve.getFrameCount(); ++i) {
    display.displayFrame(mazeSolve.getNextFrame(), "maze");
    display.sleep(15);
    display.clearScreen();
  }
  int returnToMenu = -1;
  while (returnToMenu != 0) {
    display.displayFrame(mazeSolve.getNextFrame(), "maze");
    display.displayFrame("Time taken to solve a maze: ");
    display.displayFrame(to_string(duration) + " milliseconds\n", "blue");
    display.displayFrame("\nPress 0 to return to main menu -> ");
    returnToMenu = display.readInput<int>();
    display.clearScreen();
  }
}
