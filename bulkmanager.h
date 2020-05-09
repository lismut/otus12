//
// Created by nether on 08.05.2020.
//

#pragma once

#include <memory>
#include "bulk.h"
#include "subscriber.h"

extern std::mutex globalCoutMutex;

/// @brief Класс-обработчик поступающих входных строк
struct bulkManager {
    bulkManager(size_t sz) : currentBulk(sz), bulkLimit(sz) {
        printers.push_back(std::make_unique<printer>());
        filers.push_back(std::make_unique<filer>(1));
        filers.push_back(std::make_unique<filer>(2));
    }
    void newString(const std::string& input);
    void finish();
    // For testing
    const bulk& getBulk() const {return currentBulk; }
    const size_t& getNesting() const {return nestingCounter; }
    ~bulkManager() {
        std::lock_guard<std::mutex> grd(globalCoutMutex);
        std::cout << "main поток - строки=" << strCounter << ", блоки=" << bulkCounter << ", команды=" << comCounter << std::endl;
    }
private:
    void flush();

    size_t nestingCounter = 0; //< Текущий уровень вложенности {
    size_t bulkLimit; //< Размер блока команд
    bulk currentBulk; //< Текущий блок команд
    std::vector<std::unique_ptr<isubscriber>> printers; //< Вектор подписчиков
    std::vector<std::unique_ptr<isubscriber>> filers; //< Вектор подписчиков
    size_t strCounter = 0;
    size_t comCounter = 0;
    size_t bulkCounter = 0;
};

void bulkManager::newString(const std::string& input) {
    static const std::string open = "{";
    static const std::string close = "}";
    strCounter++;
    if (input != open && input != close) {
        currentBulk.push(input);
        comCounter++;
        if (currentBulk.size() == bulkLimit && nestingCounter == 0) {
            flush();
        }
    } else if (input == open) {
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
    std::for_each(printers.begin(), printers.end(), [&](const auto& a){
        a->update(currentBulk);
    });
    filers[bulkCounter%filers.size()]->update(currentBulk);
    bulkCounter++;
    currentBulk.clear();
}