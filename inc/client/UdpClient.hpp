/*
** EPITECH PROJECT, 2019
** udpclient
** File description:
** udp
*/

#pragma once

#define MAX_SIZE 8192

#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <queue>

class UdpClient : public std::enable_shared_from_this<UdpClient>
{
public:
    std::vector<std::vector<unsigned char>> &_audioData;
    
    UdpClient(boost::asio::io_context &io_context, std::string host, std::string port1, std::string port2, std::vector<std::vector<unsigned char>> &p_audioData);
    void sendStart(std::vector<unsigned char> message);
    void receiveStart(void);

private:
    void handlerReceive(const boost::system::error_code& error);
    void handlerSend(const boost::system::error_code &error);

    boost::asio::ip::udp::socket _socketUdp;
    std::vector<unsigned char> _data;
    boost::asio::ip::udp::endpoint _remoteEndpoint;
    boost::asio::ip::udp::endpoint _senderEndpoint;
    boost::array<char, 1> _recvBuffer;
    char *_buff[MAX_SIZE];
    int _onCall = 1;
    int _writing = 0;
    std::queue<std::vector<unsigned char>> _sendQueue;
};
