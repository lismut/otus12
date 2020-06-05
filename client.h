//
// Created by nether on 11.05.2020.
//

#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
namespace ba = boost::asio;

typedef std::deque<std::string> message_queue;

class client {
public:
    client(ba::io_service& _io_service,
                tcp::resolver::iterator endpoint_iterator)
            : io_service(_io_service),
              socket(_io_service)
    {
        do_connect(endpoint_iterator);
    }

    void write(const std::string& msg)
    {
//        io_service.post(
//                [this, msg]()
//                {
                    bool write_in_progress = !write_msgs.empty();
                    write_msgs.push_back(msg);
                    std::cout << "quered " << msg  << " len=" << write_msgs.front().length() << std::endl;
                    if (!write_in_progress) {
                        do_write();
                    }
//                });
    }

    void close()
    {
        std::cout << "client disconnected" << std::endl;
        io_service.post([this]() { socket.close(); });
    }

private:
    void do_connect(tcp::resolver::iterator endpoint_iterator)
    {
        ba::async_connect(socket, endpoint_iterator,
                                   [this](boost::system::error_code ec, tcp::resolver::iterator)
                                   {
                                       if (!ec) {
                                           std::cout << "connected" << std::endl;
                                       }
                                   });
    }

    void do_write() {
        size_t len = write_msgs.front().length();
        auto aa = write_msgs.front().data();
        write_msgs.pop_front();
        ba::async_write(socket,
                                 ba::buffer(aa, len),
                                 [this](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec) {
                                         std::cout << "sent " << write_msgs.front().data() << " len=" << write_msgs.front().length() << std::endl;
                                         if (!write_msgs.empty()) {
                                             do_write();
                                         } else {

                                         }
                                     } else {
                                         std::cout << "ec on write " << std::endl;
                                         socket.close();
                                     }
                                 });
    }

private:
    ba::io_service& io_service;
    tcp::socket socket;
    //std::string read_msg;
    //std::array<char, 1024> reply;
    message_queue write_msgs;
};

