#include <iostream>
#include "bulkmanager.h"
#include <mutex>

std::mutex globalCoutMutex;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage:" << std::endl;
        std::cout << "   bulk [number of commands]" << std::endl;
        //return 0;
    }
    std::string one;
    bulkManager manager(3);//atoi(argv[1]));
    while (std::cin >> one) {
        if(std::cin.eof() || one =="quit") break;
        manager.newString(one);
    }
    manager.finish();
    return 0;
}
