#pragma once

#include <string>

// Minimal application configuration, loaded from a config.toml file
// sitting next to the executable. Only a small, flat subset of TOML
// is supported: "key = value" lines (ints, floats, bools, quoted
// strings), "#" comments, blank lines, and "[section]" headers which
// are parsed but otherwise ignored (all keys are looked up by their
// bare name regardless of section).
struct Config {
    unsigned int windowWidth = 1024;
    unsigned int windowHeight = 768;
    bool startFullscreen = false;

    // Attempts to load config.toml from `path`. If the file can't be
    // opened, or a key is missing/malformed, the corresponding
    // field(s) keep their default value(s) above. Always returns a
    // usable Config.
    static Config load(const std::string &path = "config.toml");
};
