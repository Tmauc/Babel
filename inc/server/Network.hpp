/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Abstraction
*/

#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <boost/asio.hpp>
#include <boost/bind.hpp>

class Handler;
class Server;

typedef std::shared_ptr<Handler> pointer;

class Network {
    public:
        Network(boost::asio::io_context &context);
        ~Network(void);
        boost::asio::ip::tcp::socket &getSocket(void);
        void asyncReadSome(char *data, int maxSize, void(Handler::*func)(const boost::system::error_code&), pointer connect);
        void asyncWriteSome(std::string data, void(Handler::*func)(const boost::system::error_code&), pointer connect);
        void close(void);
        std::string getIP(void);

    protected:
    private:
        boost::asio::ip::tcp::socket _socket;
};

class Acceptor {
    public:
        Acceptor(void);
        ~Acceptor(void);
        boost::asio::io_context &getContext(void);
        void run();
        void asyncAccept(Network &net, void(Server::*func)(pointer, const boost::system::error_code&), Server *mainSrv, pointer newConnect);

    protected:
    private:
        boost::asio::io_context _context;
        boost::asio::ip::tcp::acceptor _acceptor;
};

#endif /* !NETWORK_HPP_ */
