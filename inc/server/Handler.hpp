/*
** EPITECH PROJECT, 2019
** babel
** File description:
** server handler
*/

#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#define MAX_SIZE 1024

#include <iostream>
#include <memory>
#include <queue>
#include "Network.hpp"

class Server;

class Handler : public std::enable_shared_from_this<Handler>
{
    typedef std::shared_ptr<Handler> pointer;
public:
    Handler(Acceptor &context, Server *mainSrv, int id);
    ~Handler(void);
    pointer create(Acceptor &context);
    Network &getSocket(void);
    void handleRead(const boost::system::error_code &error);
    void handleWrite(const boost::system::error_code &error);
    void start(void);
    void writeSome(std::string msg);
    void parseMessage(std::string data);
    void newUser(std::string data);
    std::string getUser(void);
    std::string getIp(void);
    int getId(void);

protected:
private:
    Server *_mainSrv;
    Network _sock;
    char _data[MAX_SIZE];
    bool _writing = 0;
    std::string _receiveBuffer;
    std::queue<std::string> _sendingBuffer;
    std::string _username;
    std::string _ip;
    int _del = 0;
    int _id;

    bool allMessageRead();
    bool set_client(std::string p_data);
    bool del_client(std::string p_data);
    bool call_client(std::string p_data);
};

#endif /* !HANDLER_HPP_ */
