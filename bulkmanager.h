//
// Created by nether on 08.05.2020.
//

#pragma once

#include <memory>
#include "bulk.h"
#include "subscriber.h"

/// @brief Класс-обработчик поступающих входных строк
struct bulkManager {
    bulkManager(size_t sz) : currentBulk(sz), bulkLimit(sz) {
        subs.push_back(std::make_unique<printer>());
        subs.push_back(std::make_unique<filer>());
    }
    void newString(const std::string& input);
    void finish();
    // For testing
    const bulk& getBulk() const {return currentBulk; }
    const size_t& getNesting() const {return nestingCounter; }
private:
    void flush();

    size_t nestingCounter = 0; //< Текущий уровень вложенности {
    size_t bulkLimit; //< Размер блока команд
    bulk currentBulk; //< Текущий блок команд
    std::vector<std::unique_ptr<isubscriber>> subs; //< Вектор подписчиков
};

void bulkManager::newString(const std::string& input) {
    if (input != "{" && input != "}") {
        currentBulk.push(input);
        if (currentBulk.size() == bulkLimit && nestingCounter == 0) {
            flush();
        }
    } else if (input == "{") {
        if (nestingCounter == 0) {
            flush();
        }
        ++nestingCounter;
    } else {
        --nestingCounter;
        if (nestingCounter < 1) {
            flush();
            nestingCounter = 0; // во избежание проблем с некорректной последовательностью закрывающих скобок
        }
    }
}

void bulkManager::finish() {
    flush();
}

/// @brief Обработка конца блока команд
void bulkManager::flush() {
    std::for_each(subs.begin(), subs.end(), [&](const auto& a){
        a->update(currentBulk);
    });
    currentBulk.clear();
}