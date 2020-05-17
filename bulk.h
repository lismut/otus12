//
// Created by nether on 08.05.2020.
//

#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

/// @brief Класс предназначенный для хранения блоков команд
struct bulk {
public:
    explicit bulk(size_t sz) { commands.reserve(sz); }
    bulk(bulk&& src) noexcept : name_time(std::move(src.name_time)), commands(std::move(src.commands)) {}
    bulk(const bulk& src) = default;
    void push(const std::string& cmd){
        if (commands.empty()) {
            std::time_t result = std::time(nullptr);
            name_time = std::to_string(result);
        }
        commands.push_back(cmd);

    }
    std::string output() const{
        std::string res;
        std::for_each(commands.begin(), commands.end(), [&](const auto& a) {
            res += a + " ";
        });
        return res;
    }
    std::string name() const { return name_time;  };
    size_t size() const { return commands.size(); }
    void clear() { commands.clear(); name_time = ""; }
private:
    std::vector<std::string> commands;
    std::string name_time;
};