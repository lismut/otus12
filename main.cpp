#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "async.h"
//#include "client.h"
using boost::asio::ip::tcp;

//----------------------------------------------------------------------
std::mutex globalCoutMutex;

class session
        : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, async::handle_t handle_)
            : socket_(std::move(socket)), handle(handle_)
    {
        qq = std::make_shared<QueueString>();
    }

    void start()
    {
        do_read();
    }
    ~session() {
        async::disconnect(handle);
    }
private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                [this, self](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        //std::cout << "receive " << length << "=" << std::string{data_, length} << std::endl;
                                        async::receive(handle, data_, length, qq);
                                        do_read();
                                    }
                                });
    }

    /*void do_write()
    {
        std::unique_ptr<std::string> one = qq->dequeue();
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(one->c_str(), one->size()),
                                 [this, self](boost::system::error_code ec, std::size_t length)
                                 {
                                     if (!ec)
                                     {
                                         if(qq->empty())
                                             do_read();
                                         else
                                             do_write();
                                     }
                                 });
    }*/

    tcp::socket socket_;
    async::handle_t handle;
    std::shared_ptr<QueueString> qq;
    enum { max_length = 1024 };
    char data_[max_length];
};
//----------------------------------------------------------------------

class server
{
public:
    server(boost::asio::io_context& io_con, short port, short _bulk_size)
            : acceptor_(io_con,tcp::endpoint(tcp::v4(), port)), bulk_size(_bulk_size)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket)
                {
                    if (!ec)
                    {
                        async::handle_t handler = async::connect(bulk_size);
                        std::make_shared<session>(std::move(socket), handler)->start();
                    }
                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
    short bulk_size;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: async_tcp_echo_server <port> <bulk_size>\n";
            return 1;
        }
        size_t bulk_size = 0;
        size_t port = 0;
        try {
            bulk_size = atoi(argv[2]);
            port = std::atoi(argv[1]);
            if (port <= 1 || bulk_size <= 1) {
                throw std::exception();
            }
        } catch (std::exception& ex) {
            std::cout << "Incorrect parameters " << ex.what() << std::endl;
            return 1;
        }

        boost::asio::io_context io_con;

        server server(io_con, port, bulk_size);

        io_con.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }


    return 0;
}