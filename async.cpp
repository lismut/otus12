#include <map>
#include <sstream>
#include "async.h"
#include "bulkmanager.h"
namespace async {
std::map<size_t , std::unique_ptr<bulkManager>> workers;

handle_t connect(std::size_t bulk) {
    size_t idx = workers.size();
    workers[idx] = std::make_unique<bulkManager>(bulk);
    return idx;
}

void receive(handle_t handle, const char *data, std::size_t size, std::shared_ptr<QueueString> qq) {
    auto it = workers.find(handle);
    if (it != workers.end()) {
        std::stringstream ss(data);
        for (auto i = 0; i < size; i++) ss << data[i];
        while (!ss.eof()) {
            std::string a;
            ss >> a;
            it->second->newString(a, qq);
        }
    } else {
        std::cout << "something went wrong with id handler" << std::endl;
    }
}

void disconnect(handle_t handle) {
    auto it = workers.find(handle);
    if (it != workers.end()) {
        workers.erase(it);
    }
}
}
