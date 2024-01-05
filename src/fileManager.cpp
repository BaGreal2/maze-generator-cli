#include "../include/fileManager.hpp"
#include "../include/constants.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

using std::ofstream, std::ifstream;
namespace fs = std::filesystem;

FileManager::FileManager() {
}

int FileManager::createSaveDirectory() {
  if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
    if(fs::create_directory(directoryPath)) {
      return 0;
    } else {
      return 1;
    }
  }
  return 0;
}

int FileManager::save(string name, string data) {
  int errorCode = createSaveDirectory();
  if(errorCode == 1) {
    display.displayFrame("Error creating a save directory!\n");
    display.sleep(50);
    return 1;
  }
  ofstream fout(directoryPath + "/" + name + SAVED_FILE_EXTENTION);
  fout << data;
  fout.close();
  return 0;
}

string FileManager::read(string name) {
  ifstream fin(directoryPath + "/" + name);
  if(fin.is_open()) {
    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string data = buffer.str();

    fin.close();
    return data;
  } else {
    display.displayFrame("Error opening a file!\n");
    display.sleep(50);
    return "";
  }
  return "";
}

vector<string> FileManager::getSavedFilesNames() {
  vector<string> files;

  try {
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
      if (fs::is_regular_file(entry.path())) {
        files.push_back(entry.path().filename().string());
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return files;
}
