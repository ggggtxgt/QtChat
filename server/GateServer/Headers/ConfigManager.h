#ifndef GATESERVER_CONFIGMANAGER_H
#define GATESERVER_CONFIGMANAGER_H

#include "const.h"

struct SectionInfo {
    SectionInfo() {}

    ~SectionInfo() { _section_datas.clear(); }

    SectionInfo(const SectionInfo &src) {
        _section_datas = src._section_datas;
    }

    SectionInfo &operator=(const SectionInfo &src) {
        if (&src == this) {
            return *this;
        }
        this->_section_datas = src._section_datas;
    }

    std::string operator[](const std::string &key) {
        if (_section_datas.end() == _section_datas.find(key)) {
            return "";
        }
        return _section_datas[key];
    }

    std::map<std::string, std::string> _section_datas;
};

class ConfigManager {
public:
    ~ConfigManager() {
        _config_map.clear();
    }

    SectionInfo operator[](const std::string &section) {
        if (_config_map.find(section) == _config_map.end()) {
            return SectionInfo();
        }
        return _config_map[section];
    }

    ConfigManager();

    ConfigManager(const ConfigManager &src) {
        _config_map = src._config_map;
    }

    ConfigManager &operator=(const ConfigManager &src) {
        if (this == &src) {
            return *this;
        }
        _config_map = src._config_map;
    }

private:
    std::map<std::string, SectionInfo> _config_map;
};

#endif //GATESERVER_CONFIGMANAGER_H
