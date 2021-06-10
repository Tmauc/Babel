/*
** EPITECH PROJECT, 2019
** babel
** File description:
** server handler
*/

#include "Server.hpp"

typedef std::shared_ptr<Handler> pointer;

Handler::Handler(Acceptor &context, Server *mainSrv, int id)
    : _sock(context.getContext()), _mainSrv(mainSrv), _id(id)
{
    std::memset(_data, '\0', MAX_SIZE);
}

Handler::~Handler(void)
{
}

Network &Handler::getSocket(void)
{
    return _sock;
}

std::string Handler::getUser(void)
{
    return _username;
}

std::string Handler::getIp(void)
{
    return _ip;
}

int Handler::getId(void)
{
    return _id;
}

void Handler::start(void)
{
    _sock.asyncReadSome(_data, MAX_SIZE, &Handler::handleRead, shared_from_this());
}

void Handler::writeSome(std::string msg)
{
    std::string size = std::to_string(msg.length());

    msg.insert(0, size);
    _sendingBuffer.push(msg);
    if (_writing == 0) {
        _writing = 1;
        _sock.asyncWriteSome(_sendingBuffer.front(), &Handler::handleWrite, shared_from_this());
    }
}

void Handler::newUser(std::string data)
{
    std::vector<std::string> allUser = _mainSrv->getAllUser(_id);

    std::cout << "[NEW USER]: "
                << data
                << std::endl;
    writeSome("ACK");
    for (auto it = allUser.begin(); it != allUser.end(); it++) {
        writeSome("NEW;" + *it);
    }
    _mainSrv->addUser("NEW;" + _username + ";" + _ip + ";" + std::to_string(_id), _id);
}

void Handler::parseMessage(std::string data)
{
    if (data.substr(0, 3) == "NEW" && this->set_client(data)) {
        newUser(data);
    } else if (data.substr(0, 3) == "DEL" && this->del_client(data)) {
        _del = 1;
        writeSome("DEL");
    } else if (data.substr(0, 3) == "CAL" && this->call_client(data)) {
        writeSome("CAK");
    } else {
        this->_sock.close();
        std::cerr << "Unknows operation by client : " << data << std::endl;
    }
}

void Handler::handleRead(const boost::system::error_code &error)
{
    if (error) {
        std::cerr << "errorR: " << error.message() << std::endl;
        _sock.close();
        return;
    }
    _receiveBuffer += _data;
    std::memset(_data, '\0', MAX_SIZE);
    if (allMessageRead()) {
        parseMessage(_receiveBuffer);
        _receiveBuffer.clear();
    }
    if (_del == 1) {
        _mainSrv->deleteUser(_id);
        this->_sock.close();
        return;
    }
    _sock.asyncReadSome(_data, MAX_SIZE, &Handler::handleRead, shared_from_this());
}

bool Handler::set_client(std::string p_data)
{
    char *data = strdup(p_data.data());
    char *neww = strtok(data, ";");
    char *username = strtok(NULL, ";");

    if (neww == NULL || username == NULL)
        return false;
    this->_username = username;
    this->_ip = this->_sock.getIP();
    return true;
}

bool Handler::del_client(std::string p_data)
{
    char *data = strdup(p_data.data());
    char *del = strtok(data, ";");
    char *username = strtok(NULL, ";");

    std::cout << "[DEL USER]: "
              << p_data
              << std::endl;
    if (del == NULL || username == NULL ||
        std::string(username) != this->_username)
        return (false);
    return true;
}

bool Handler::call_client(std::string p_data)
{
    std::string callingId;
    std::string port;

    p_data = p_data.substr(4);
    callingId.append(p_data.substr(0, p_data.find(';')));
    p_data = p_data.substr(p_data.find(';') + 1);
	port.append(p_data.substr(0, p_data.find(';')));

    if (callingId.length() == 0 || p_data.length() == 0)
        return false;
    _mainSrv->sendToId(std::stoi(callingId), "CAL;" + _sock.getIP() + ";" + port + ";");
    return true;
}

void Handler::handleWrite(const boost::system::error_code &error)
{
    if (error) {
        std::cerr << "error: " << error.message() << std::endl;
        _sock.close();
        return;
    }
    _sendingBuffer.pop();
    if (!_sendingBuffer.empty()) {
        _sock.asyncWriteSome(_sendingBuffer.front(), &Handler::handleWrite, shared_from_this());
    } else {
        _writing = 0;
    }
}

bool Handler::allMessageRead()
{
    int size = std::stoi(_receiveBuffer);

    if (_receiveBuffer.substr(std::to_string(size).length()).length() == size) {
        _receiveBuffer = _receiveBuffer.substr(std::to_string(size).length());
        return true;
    }
    return false;
}
