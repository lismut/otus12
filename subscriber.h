//
// Created by nether on 08.05.2020.
//

#pragma once

#include "bulk.h"
#include "fstream"

///  @brief Класс-интерфейс подписчиков для обработчиков конца блока команд
class isubscriber {
public:
    virtual void update(const bulk& b) = 0;
};

///  @brief Класс-подписчик для вывода на экран
class printer : public isubscriber {
    void update(const bulk& b) final{
        std::cout << "bulk: " << b.output() << std::endl;
    }
};

///  @brief Класс-подписчик для вывода в файл
class filer : public isubscriber {
    void update(const bulk& b) final{
        std::ofstream a("bulk" + b.name());
        a << b.output() << std::endl;
        a.close();
    }
};