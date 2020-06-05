//
// Created by nether on 11.05.2020.
//

#include "client.h"

void start_client(const std::string& host, const std::string& port)
{
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ host, port });
    client c1(io_service, endpoint_iterator);
    std::thread t([&io_service](){ io_service.run(); });

    std::string msg;
    while (std::cin >> msg) {
        c1.write(msg);
        if (msg == "quit") break;
    }
    c1.close();
    t.join();
}

int main(int argc, char* argv[])
{
    try {
        if (argc != 3) {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }
        start_client(argv[1], argv[2]);
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}