//
// Created by nether on 08.05.2020.
//

#pragma once

#include "bulk.h"
#include <fstream>
#include <thread>
#include "threadqueue.h"

extern std::mutex globalCoutMutex;

///  @brief Класс-интерфейс подписчиков для обработчиков конца блока команд
class isubscriber {
public:
    explicit isubscriber(std::string&& _name) : name(_name) {}
    void update(const bulk& b) {
        comCount += b.size();
        blocksCount++;
        tasks.enqueue(std::make_unique<bulk>(b));
    }
    ~isubscriber() {
        isFinish = true;
        my->join();
        std::lock_guard<std::mutex> grd(globalCoutMutex);
        std::cout << name << " поток - блоки=" << blocksCount << ", команды=" << comCount << std::endl;
    }
protected:
    Queue tasks;
    size_t blocksCount = 0;
    size_t comCount = 0;
    std::string name;
    std::unique_ptr<std::thread> my;
    std::atomic<bool> isFinish{false};
};

///  @brief Класс-подписчик для вывода на экран
class printer : public isubscriber {
public:
    explicit printer() : isubscriber("log") {
        my = std::make_unique<std::thread>([&](){
            while(!isFinish){
                if(!tasks.empty()) {
                    proc(tasks.dequeue());
                }
            }
        });
    }
private:
    void proc(std::unique_ptr<bulk> b) {
        std::lock_guard<std::mutex> grd(globalCoutMutex);
        std::cout << "bulk: " << b->output() << std::endl;
    }
};

///  @brief Класс-подписчик для вывода в файл
class filer : public isubscriber {
public:
    explicit filer(size_t _idx) : idx(_idx), isubscriber("file" + std::to_string(_idx)) {
        my = std::make_unique<std::thread>([&](){
            while(!isFinish){
                if(!tasks.empty()) {
                    proc(tasks.dequeue());
                }
            }
        });

    }
private:
    size_t idx;

    void proc(std::unique_ptr<bulk> b) const {
        std::ofstream a("bulk" + b->name() + "id" + std::to_string(idx));
        a << b->output() << std::endl;
        a.close();
    }
};