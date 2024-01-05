#ifndef DISPLAYMANAGER_HPP
#define DISPLAYMANAGER_HPP

#include <iostream>
#include <string>

using std::string;

class DisplayManager {
private:
  string currentFrame;
  string red(string text);
  string blue(string text);
  string green(string text);
  string yellow(string text);

public:
  DisplayManager();
  void clearScreen();
  void displayFrame(string frame, string mode = "");
  void sleep(int ms);
  template <typename T> T readInput() {
    T userInput;
    std::cin >> userInput;
    return userInput;
  }
};

#endif
