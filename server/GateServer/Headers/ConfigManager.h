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
        return *this;
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

    ConfigManager(const ConfigManager &src) {
        _config_map = src._config_map;
    }

    ConfigManager &operator=(const ConfigManager &src) {
        if (this == &src) {
            return *this;
        }
        _config_map = src._config_map;
    }

    static ConfigManager &Inst() {
        // 局部静态变量的生命周期与进程同步，但其作用域仅限于局部（C++11标准之后）
        static ConfigManager cfg_mgr;
        return cfg_mgr;
    }

private:
    ConfigManager();

private:
    std::map<std::string, SectionInfo> _config_map;
};

#endif //GATESERVER_CONFIGMANAGER_H
