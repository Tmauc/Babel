/*
** EPITECH PROJECT, 2019
** babelclient.hpp
** File description:
** babel
*/

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <thread>
#include <future>
#include <memory>
#include <queue>
#include <utility>

#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include <opus.h>

#include "PortAudio.hpp"
#include "Opus.hpp"
#include "UdpClient.hpp"

#define TIMEOUT_SEC (5)

class BabelWindow;

enum class PROTOCOL
{
    ERROR,
    REQ_LOGIN,
    ACK_LOGIN,
    REQ_LOGOUT,
    ACK_LOGOUT
};

class BabelClient : public std::enable_shared_from_this<BabelClient>
{
public:
    BabelClient(std::string p_host, std::string p_port, BabelWindow *mainWindow);
    ~BabelClient(void);

    bool login(std::string username);
    bool logout(std::string username);
    bool start_call(void);
    void end_call(void);
    void writeMessage(std::string msg);
    void startAccept(const boost::system::error_code& err);
    void startSend(void);
    void readSock(const boost::system::error_code &error);
    void writeSock(const boost::system::error_code &error);
    std::string readMessage(void);
    void parseMsg(std::string msg);
    void addNewUser(std::string newUser);
    void delContact(std::string outUser);
    void call_someone(std::string p_username, std::string p_port);

    boost::asio::io_context _io_context;
    std::map<std::string, std::pair<std::string, std::string>> getAllUser(void) { return _allUser; }

private:
    BabelWindow *_mainWindow;
    PortAudio *_pa;
    Opus *_o;
    UdpClient *_udp;
    boost::asio::ip::tcp::socket _socket;
    int _closing = 0;
    char _data[MAX_SIZE];
    std::thread _t1;
    bool _writing = 0;
    std::string _receiveBuffer;
    std::queue<std::string> _sendingBuffer;
    int _answer = -1;
    int _del = 0;
    std::map<std::string, std::pair<std::string, std::string>> _allUser;

    bool allMessageRead();
    void get_called(std::string p_msg);
};
