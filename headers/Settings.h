#pragma once
#include <fstream>
#include <string>

class Settings {
 private:
  bool isShaders = true;
  bool isVsync = true;
  bool isSFX = true;
  bool isFullScreen = true;
  const char filename[11] = "config.inf";

  std::string boolToString(bool b) {
    if (b) {
      return "true";
    } else {
      return "false";
    }
  }

 public:
  Settings() {
    // load settings
    std::ifstream configFile;
    configFile.open(filename);
    if (configFile.is_open()) {
      std::string s;
      configFile >> s >> s;
      isShaders = (s == "true");
      configFile >> s >> s;
      isVsync = (s == "true");
      configFile >> s >> s;
      isSFX = (s == "true");
      configFile >> s >> s;
      isFullScreen = (s == "true");
    }
    // use default values otherwise
  }

  ~Settings() {
    // save settings
    std::ofstream configFile;
    configFile.open(filename);
    if (configFile.is_open()) {
      configFile << "Shaders " << boolToString(isShaders) << std::endl;
      configFile << "Vsync " << boolToString(isVsync) << std::endl;
      configFile << "SFX " << boolToString(isSFX) << std::endl;
      configFile << "Fullscreen " << boolToString(isFullScreen) << std::endl;
    }
  }

  // setters
  void setShaders(bool use) { isShaders = use; }
  void setVsync(bool use) { isVsync = use; }
  void setSFX(bool use) { isSFX = use; }
  void setFullScreen(bool use) { isFullScreen = use; }
  // getters
  bool useShaders() { return isShaders; }
  bool useVsync() { return isVsync; }
  bool useSFX() { return isSFX; }
  bool useFullScreen() { return isFullScreen; }
  bool* SFXPtr() { return &isSFX; }

  bool quit = false;
};