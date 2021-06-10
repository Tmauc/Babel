/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** Abstraction
*/

#include "Server.hpp"

Network::Network(boost::asio::io_context &context): _socket(context)
{
}

Network::~Network(void)
{
}

boost::asio::ip::tcp::socket &Network::getSocket(void)
{
    return _socket;
}

void Network::asyncReadSome(char *data, int maxSize, void(Handler::*func)(const boost::system::error_code&), pointer connect)
{
    _socket.async_read_some(boost::asio::buffer(data, maxSize),
    boost::bind(func, connect, boost::asio::placeholders::error));
}

void Network::asyncWriteSome(std::string data, void(Handler::*func)(const boost::system::error_code&), pointer connect)
{
    _socket.async_write_some(boost::asio::buffer(data, data.length()),
    boost::bind(func, connect, boost::asio::placeholders::error));
}

void Network::close(void)
{
    _socket.close();
}

std::string Network::getIP(void)
{
    return _socket.remote_endpoint().address().to_string();
}

Acceptor::Acceptor(void): _acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234))
{
}

Acceptor::~Acceptor(void)
{
}

boost::asio::io_context &Acceptor::getContext(void)
{
    return _context;
}

void Acceptor::run()
{
    _context.run();
}

void Acceptor::asyncAccept(Network &net, void(Server::*func)(pointer, const boost::system::error_code&), Server *mainSrv, pointer newConnect)
{
    _acceptor.async_accept(net.getSocket(),
        boost::bind(func, mainSrv, newConnect, boost::asio::placeholders::error));
}
