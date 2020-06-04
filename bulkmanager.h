//
// Created by nether on 08.05.2020.
//

#pragma once

#include <memory>
#include "subscriber.h"

/// @brief Класс-обработчик поступающих входных строк
struct bulkManager {
    bulkManager(size_t sz) : currentBulk(sz), bulkLimit(sz) {
        printers.push_back(std::make_unique<printer>());
        filers.push_back(std::make_unique<filer>(1));
        filers.push_back(std::make_unique<filer>(2));
    }
    void newString(const std::string& input, std::shared_ptr<QueueString> qq){
        static const std::string open = openStr();
        static const std::string close = closeStr();
        strCounter++;
        if (qq != qstring) qstring = qq;
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
    // For testing
    const bulk& getBulk() const {return currentBulk; }
    const size_t& getNesting() const {return nestingCounter; }
    ~bulkManager() {
        if (currentBulk.size() != 0) flush();
        std::lock_guard<std::mutex> grd(globalCoutMutex);
        std::string out = "main поток - строки=" + std::to_string(strCounter) + ", блоки=" + std::to_string(bulkCounter) + ", команды=" +
                std::to_string(comCounter) + "\n";
        qstring->enqueue(std::make_unique<std::string>(out));
    }
private:
    /// @brief Обработка конца блока команд
    void flush(){
        std::for_each(printers.begin(), printers.end(), [&](const auto& a){
            a->update(currentBulk, qstring);
        });
        filers[bulkCounter%filers.size()]->update(currentBulk, qstring);
        bulkCounter++;
        currentBulk.clear();
    }
    std::shared_ptr<QueueString> qstring;
    size_t nestingCounter = 0; //< Текущий уровень вложенности {
    size_t bulkLimit; //< Размер блока команд
    bulk currentBulk; //< Текущий блок команд
    std::vector<std::unique_ptr<isubscriber>> printers; //< Вектор подписчиков
    std::vector<std::unique_ptr<isubscriber>> filers; //< Вектор подписчиков
    size_t strCounter = 0;
    size_t comCounter = 0;
    size_t bulkCounter = 0;
};