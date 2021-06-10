/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <vector>

#include "Handler.hpp"

typedef std::shared_ptr<Handler> pointer;

class Server {
private:
    Acceptor *_acceptor;
    std::vector<pointer> _clients;
    int _ids = 0;

public:
    Server(Acceptor *acceptor);
    ~Server(void);
    void startAccept(void);
    void handleAccept(pointer connection, const boost::system::error_code &err);
    std::vector<std::string> getAllUser(int id);
    void deleteUser(int id);
    void addUser(std::string msg, int id);
    void sendToId(int id, std::string msg);
};

#endif /* !SERVER_HPP_ */
