#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "constants.hpp"
#include "displayManager.hpp"
#include <string>
#include <vector>

using std::string, std::vector;

class FileManager {
private:
  string directoryPath = SAVE_DIR;
  DisplayManager display;

public:
  FileManager();
  int createSaveDirectory();
  int save(string name, string data);
  string read(string name);
  vector<string> getSavedFilesNames();
};

#endif
