#include "../../include/app/Config.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace {
    std::string trim(const std::string &s) {
        const auto begin = std::find_if_not(s.begin(), s.end(), [](unsigned char c) {
            return std::isspace(c);
        });
        const auto end = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char c) {
            return std::isspace(c);
        }).base();

        if (begin >= end) {
            return "";
        }
        return {begin, end};
    }

    std::string stripComment(const std::string &line) {
        bool inQuotes = false;
        for (std::size_t i = 0; i < line.size(); ++i) {
            if (line[i] == '"') {
                inQuotes = !inQuotes;
            } else if (line[i] == '#' && !inQuotes) {
                return line.substr(0, i);
            }
        }
        return line;
    }

    std::string unquote(std::string value) {
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }
        return value;
    }

    bool toBool(const std::string &value, bool fallback) {
        std::string lower = value;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

        if (lower == "true") return true;
        if (lower == "false") return false;
        return fallback;
    }

    unsigned int toUInt(const std::string &value, unsigned int fallback) {
        try {
            const int parsed = std::stoi(value);
            if (parsed < 0) {
                return fallback;
            }
            return static_cast<unsigned int>(parsed);
        } catch (...) {
            return fallback;
        }
    }
}

Config Config::load(const std::string &path) {
    Config config;

    std::ifstream file(path);
    if (!file.is_open()) {
        return config;
    }

    std::unordered_map<std::string, std::string> values;

    std::string rawLine;
    while (std::getline(file, rawLine)) {
        const std::string line = trim(stripComment(rawLine));

        if (line.empty() || line.front() == '[') {
            // Blank line, comment-only line, or a "[section]" header.
            // Sections are ignored; all keys are treated as flat.
            continue;
        }

        const std::size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos) {
            continue;
        }

        const std::string key = trim(line.substr(0, equalsPos));
        const std::string value = trim(line.substr(equalsPos + 1));

        if (key.empty() || value.empty()) {
            continue;
        }

        values[key] = unquote(value);
    }

    if (const auto it = values.find("width"); it != values.end()) {
        config.windowWidth = toUInt(it->second, config.windowWidth);
    }

    if (const auto it = values.find("height"); it != values.end()) {
        config.windowHeight = toUInt(it->second, config.windowHeight);
    }

    if (const auto it = values.find("fullscreen"); it != values.end()) {
        config.startFullscreen = toBool(it->second, config.startFullscreen);
    }

    return config;
}
