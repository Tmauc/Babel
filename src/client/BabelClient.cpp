/*
** EPITECH PROJECT, 2019
** BabelClient
** File description:
** babel
*/

#include "BabelWindow.hpp"

BabelClient::BabelClient(std::string p_host, std::string p_port, BabelWindow *mainWindow)
    : _socket(_io_context), _mainWindow(mainWindow)
{
    std::memset(_data, '\0', MAX_SIZE);
    _socket.async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(p_host),
                                                  std::stoi(p_port)), boost::bind(&BabelClient::startAccept, this, boost::asio::placeholders::error));
    _t1 = std::thread([this](){
        this->_io_context.run();
    });
};

BabelClient::~BabelClient(void)
{
    _socket.close();
    _io_context.stop();
    _closing = 1;
    _t1.join();
};

void BabelClient::parseMsg(std::string msg)
{
    if (msg.compare(0, 3, "ACK") == 0) {
        _answer = 1;
    } else if (msg.compare(0, 3, "DEL") == 0) {
        _answer = 1;
        _del = 1;
    } else if (msg.compare(0, 3, "NEW") == 0) {
        addNewUser(msg);
        _mainWindow->updateContact(_allUser);
    } else if (msg.compare(0, 3, "OUT") == 0) {
        delContact(msg);
        _mainWindow->updateContact(_allUser);
    } else if (msg.compare(0, 3, "CAK") == 0) { // demande d'appel du serveur
        _answer = 1;
    } else if (msg.compare(0, 3, "CAL") == 0) {
        get_called(msg);
    }

}

void BabelClient::addNewUser(std::string newUser)
{
    std::string nameBuffer;
    std::string ipBuffer;
    std::pair<std::string, std::string> buff;

    newUser = newUser.substr(4);
    nameBuffer.append(newUser.substr(0, newUser.find(';')));
    newUser = newUser.substr(newUser.find(';') + 1);
    ipBuffer.append(newUser.substr(0, newUser.find(';')));
    newUser = newUser.substr(newUser.find(';') + 1);
    buff.first = nameBuffer;
    buff.second = ipBuffer;
    _allUser[newUser] = buff;
}

void BabelClient::delContact(std::string outUser)
{
    outUser = outUser.substr(4);
    _allUser.erase(outUser);
}

bool BabelClient::login(std::string username)
{
    writeMessage("NEW;" + username + ";");
    while (_answer == -1);
    if (_answer == 0) {
        _answer = -1;
        return false;
    }
    _answer = -1;
    return true;
};

bool BabelClient::logout(std::string username)
{
    writeMessage("DEL;" + username + ";");
    while (_answer == -1);
    if (_answer == 0) {
        _answer = -1;
        return false;
    }
    _answer = -1;
    return true;
};

void BabelClient::startAccept(const boost::system::error_code& err)
{
    _socket.async_read_some(boost::asio::buffer(_data, MAX_SIZE),
        boost::bind(&BabelClient::readSock, this,
        boost::asio::placeholders::error));
}

void BabelClient::writeMessage(std::string msg)
{
    std::string size = std::to_string(msg.length());

    msg.insert(0, size);
    _sendingBuffer.push(msg);
    if (_writing == 0) {
        _writing = 1;
        _socket.async_write_some(
            boost::asio::buffer(_sendingBuffer.front(), _sendingBuffer.front().length()),
            boost::bind(&BabelClient::writeSock,
                this,
                boost::asio::placeholders::error));
    }
}

void BabelClient::readSock(const boost::system::error_code &error)
{
    if (error) {
        std::cerr << "error: " << error.message() << std::endl;
        _socket.close();
        return;
    }
    _receiveBuffer += _data;
    std::memset(_data, '\0', MAX_SIZE);
    if (allMessageRead()) {
        _receiveBuffer.clear();
    }
    if (_del == 1) {
        return;
    }
    _socket.async_read_some(
        boost::asio::buffer(_data, MAX_SIZE),
        boost::bind(&BabelClient::readSock,
            this,
            boost::asio::placeholders::error));
}

void BabelClient::writeSock(const boost::system::error_code &error)
{
    if (error) {
        std::cerr << "error: " << error.message() << std::endl;
        _socket.close();
        return;
    }
    _sendingBuffer.pop();
    if (!_sendingBuffer.empty()) {
        _socket.async_write_some(
            boost::asio::buffer(_sendingBuffer.front(), _sendingBuffer.front().length()),
            boost::bind(&BabelClient::writeSock,
                this,
                boost::asio::placeholders::error));
    } else {
        _writing = 0;
    }
}

bool BabelClient::allMessageRead()
{
    int size = 0;

    while (_receiveBuffer.length() != 0) {
        size = std::stoi(_receiveBuffer);
        if (_receiveBuffer.substr(std::to_string(size).length()).length() >= size) {
            _receiveBuffer = _receiveBuffer.substr(std::to_string(size).length());
            parseMsg(_receiveBuffer.substr(0, size));
            _receiveBuffer = _receiveBuffer.substr(size);
        } else {
            return false;
        }
    }
    return true;
}

bool BabelClient::start_call(void)
{
    this->_pa = new PortAudio;
    this->_o = new Opus;

    if (!this->_pa->createStreams(this->_o->_encoder, this->_o->_decoder, this->_udp)) {
        std::cerr << "Can't create stream" << std::endl;
        throw std::exception();
    }

    if (!this->_pa->startStreams()) {
        std::cerr << "Can't start stream" << std::endl;
        throw std::exception();
    }
    return true;
}

void BabelClient::call_someone(std::string p_username, std::string p_port)
{
    boost::asio::io_context io_context;
    writeMessage("CAL;" + _allUser.begin()->first + ";" + p_port + ";");
    while (_answer == -1);
    if (_answer == 0) {
        _answer = -1;
        return;
    }
    _answer = -1;
    this->_udp = new UdpClient(io_context, _allUser.begin()->second.second, p_port, std::to_string(std::stoi(p_port) + 1), this->_pa->_audioData);
    this->start_call();
	this->_udp->receiveStart();
    // LANCER LA FENETRE DE CALL
    io_context.run();
}

void BabelClient::get_called(std::string p_msg)
{
    char *data = strdup(p_msg.data());
    char *cal = strtok(data, ";");
	char *ip = strtok(NULL, ";");
    char *port = strtok(NULL, ";");
    boost::asio::io_context io_context;

    if (cal == NULL || ip == NULL || port == NULL)
        return;
    this->_udp = new UdpClient(io_context, ip, std::to_string(std::stoi(port) + 1), port, this->_pa->_audioData);
    this->start_call();
	this->_udp->receiveStart();
    // LANCER LA FENETRE DE CALL
    io_context.run();
}

void BabelClient::end_call(void)
{
    this->_pa->stopStreams();
    delete this->_udp;
}
