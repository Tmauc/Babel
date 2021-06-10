/*
** EPITECH PROJECT, 2019
** babel
** File description:
** server
*/

#include "Server.hpp"

typedef std::shared_ptr<Handler> pointer;

Server::Server(Acceptor *acceptor): _acceptor(acceptor)
{
    startAccept();
}

Server::~Server(void)
{
}

void Server::startAccept(void)
{
    pointer connection = pointer(new Handler(*_acceptor, this, _ids));

    _ids++;
    _acceptor->asyncAccept(connection->getSocket(),
        &Server::handleAccept, this, connection);
    this->_clients.push_back(connection);
}

void Server::handleAccept(pointer connection, const boost::system::error_code& err)
{
    try {
        if (!err) {
            connection->start();
        }
    } catch (std::exception &e) {
        std::cerr << "[ERR]: Unknow operation by client" << std::endl;
    }
    startAccept();
}

std::vector<std::string> Server::getAllUser(int id)
{
    std::vector<std::string> allUser;

    for (auto it = _clients.begin(); it != _clients.end() - 1; it++) {
        if ((*it)->getId() != id) {
            allUser.push_back((*it)->getUser() + ";" + (*it)->getIp() + ";" + std::to_string((*it)->getId()));
        }
    }
    return allUser;
}

void Server::deleteUser(int id)
{
    int buffer = 0;
    int i = 0;

    for (auto it = _clients.begin(); it != _clients.end() - 1; it++, i++) {
        if ((*it)->getId() == id) {
            buffer = i;
        } else {
            (*it)->writeSome("OUT;" + std::to_string(id));
        }
    }
    _clients.erase(_clients.begin() + buffer);
}

void Server::addUser(std::string msg, int id)
{
    for (auto it = _clients.begin(); it != _clients.end() - 1; it++) {
        if ((*it)->getId() != id) {
            (*it)->writeSome(msg);
        }
    }
}

void Server::sendToId(int id, std::string msg)
{
    for (auto it = _clients.begin(); it != _clients.end() - 1; it++) {
        if ((*it)->getId() == id) {
            (*it)->writeSome(msg);
            return;
        }
    }
}