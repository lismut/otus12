//
// Created by nether on 08.05.2020.
//

#pragma once

#include <fstream>
#include <iostream>
#include <thread>
#include "threadqueue.h"

extern std::mutex globalCoutMutex;

///  @brief Класс-интерфейс подписчиков для обработчиков конца блока команд
class isubscriber {
public:
    explicit isubscriber(std::string&& _name) : name(_name) {}
    void update(const bulk& b, std::shared_ptr<QueueString> qq) {
        if (qstring != qq) qstring = qq;
        comCount += b.size();
        blocksCount++;
        tasks.enqueue(std::make_unique<bulk>(b));
    }
    ~isubscriber() {
        isFinish = true;
        my->join();
    }
protected:
    Queue tasks;
    std::shared_ptr<QueueString> qstring;
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
            std::lock_guard<std::mutex> grd(globalCoutMutex);
            std::string out = name + " поток - блоки=" + std::to_string(blocksCount) + ", команды=" + std::to_string(comCount) + "\n";
            qstring->enqueue(std::make_unique<std::string>(out));
        });
    }
private:
    void proc(std::unique_ptr<bulk> b) {
        std::lock_guard<std::mutex> grd(globalCoutMutex);
        std::string out = "bulk: " + b->output() + "\n";
        qstring->enqueue(std::make_unique<std::string>(out));
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