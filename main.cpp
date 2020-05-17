#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
//#include "client.h"
using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<std::string> message_queue;

//----------------------------------------------------------------------

class participant
{
public:
    virtual ~participant() {}
    virtual void deliver(const std::string& msg) = 0;
};

typedef std::shared_ptr<participant> participant_ptr;

//----------------------------------------------------------------------

class room
{
public:
    void join(participant_ptr _participant)
    {
        participants.insert(_participant);
        std::cout << "joined to room" << std::endl;
        for (auto msg: recent_msgs)
            _participant->deliver(msg);
    }

    void leave(participant_ptr participant)
    {
        participants.erase(participant);
    }

    void deliver(const std::string& msg)
    {
        std::cout << "went to deliver in room" << msg << std::endl;
        recent_msgs.push_back(msg);
        while (recent_msgs.size() > max_recent_msgs)
            recent_msgs.pop_front();

        for (auto participant: participants)
            participant->deliver(msg);
    }

private:
    std::set<participant_ptr> participants;
    enum { max_recent_msgs = 100 };
    message_queue recent_msgs;
};

//----------------------------------------------------------------------

class session
        : public participant,
          public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket _socket, room& _room)
            : socket(std::move(_socket)),
              roomI(_room)
    {
    }

    void start()
    {
        roomI.join(shared_from_this());
        roomI.deliver("hihi");
        do_read();
    }

    void deliver(const std::string& msg) final
    {
        std::cout << "went to deliver in session " << msg << std::endl;
        bool write_in_progress = !write_msgs.empty();
        write_msgs.push_back(msg);
        if (!write_in_progress) {
            do_write();
        }
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        read_msg.reserve(100);
        std::cout << "async read started" << std::endl;
        boost::asio::async_read(socket,
                                boost::asio::buffer(reply.data(), reply.size()),
                                [this, self](boost::system::error_code ec, std::size_t length)
                                {
                                    std::cout << "async read called " << length << std::endl;
                                    if (!ec) {
                                        std::cout << "received: ";
                                        std::cout.write(reply.data(), length);
                                        std::cout << std::endl;
                                        read_msg = std::string(reply.data(), length);
                                        std::cout << "transform to string: " << read_msg << std::endl;
                                        roomI.deliver("read_msg");
                                        do_read();
                                    } else {
                                        std::cout << "leaved"<< std::endl;
                                        roomI.leave(shared_from_this());
                                    }
                                });
    }

    void do_write()
    {
        std::cout << "async write started" << std::endl;
        auto self(shared_from_this());
        boost::asio::async_write(socket,
                                 boost::asio::buffer(write_msgs.front().data(),write_msgs.front().length()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     std::cout << "async write called " << std::endl;
                                     if (!ec) {
                                         std::cout << "sent: " << write_msgs.front() << " size: " << write_msgs.front().length() << std::endl;
                                         write_msgs.pop_front();
                                         if (!write_msgs.empty()) {
                                             do_write();
                                         } else {
                                             do_read();
                                         }
                                     } else {
                                         std::cout << "leaved"<< std::endl;
                                         roomI.leave(shared_from_this());
                                     }
                                 });
    }

    tcp::socket socket;
    room& roomI;
    std::array<char, 1024> reply;
    std::string read_msg;
    message_queue write_msgs;
};

//----------------------------------------------------------------------

class server
{
public:
    server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint)
            : acceptor(io_service, endpoint),
              socket(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor.async_accept(socket,
                               [this](boost::system::error_code ec)
                               {
                                   if (!ec) {
                                       std::cout << "new session connected" << std::endl;
                                       std::make_shared<session>(std::move(socket), roomI)->start();
                                   }
                                   do_accept();
                               });

    }

    tcp::acceptor acceptor;
    tcp::socket socket;
    room roomI;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_service io_service;


        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
        //tcp::endpoint endpoint2(tcp::v4(), std::atoi(argv[2]));
        server servers(io_service, endpoint);
       // server servers2(io_service, endpoint2);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}