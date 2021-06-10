/*
** EPITECH PROJECT, 2019
** UdpClient
** File description:
** udp
*/

#include "UdpClient.hpp"

UdpClient::UdpClient(boost::asio::io_context &io_context, std::string host, std::string port1, std::string port2, std::vector<std::vector<unsigned char>> &p_audioData)
    : _audioData(p_audioData), _socketUdp(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), std::stoi(port1)))
{
    std::cout << port1 << " & " << port2 << std::endl;
    boost::asio::ip::udp::resolver resolver(io_context);
    boost::asio::ip::udp::resolver::iterator itr = resolver.resolve(boost::asio::ip::udp::v4(), host, port2);

    _remoteEndpoint = *itr;
    _data.clear();
    std::cout << "call" << std::endl;
}

void UdpClient::receiveStart(void)
{
    _socketUdp.async_receive_from(
        boost::asio::buffer(_data, MAX_SIZE), _senderEndpoint,
        boost::bind(&UdpClient::handlerReceive, this,
        boost::asio::placeholders::error));
}

void UdpClient::sendStart(std::vector<unsigned char> message)
{
    _sendQueue.push(message);
    if (_writing == 0) {
        _writing = 1;
        _socketUdp.async_send_to(boost::asio::buffer(_sendQueue.front(), _sendQueue.front().size()), _remoteEndpoint,
            boost::bind(&UdpClient::handlerSend, this,
            boost::asio::placeholders::error));
    }
}

void UdpClient::handlerReceive(const boost::system::error_code& error)
{
    if (error) {
        std::cerr << "errorR: " << error.message() << std::endl;
        _socketUdp.close();
        return;
    } else if (_onCall == 0) {
        _socketUdp.close();
        return;
    }
    std::cout << "RECEIVE" << std::endl;
	_audioData.push_back(_data);
    _data.clear();
    _socketUdp.async_receive_from(
        boost::asio::buffer(_data, MAX_SIZE), _senderEndpoint,
        boost::bind(&UdpClient::handlerReceive, this,
        boost::asio::placeholders::error));
}

void UdpClient::handlerSend(const boost::system::error_code &error)
{
    if (error) {
        std::cerr << "errorW: " << error.message() << std::endl;
        _socketUdp.close();
        return;
    } else if (_onCall == 0) {
        _socketUdp.close();
        return;
    }
	std::cout << "SEND" << std::endl;
    _sendQueue.pop();
    if (!_sendQueue.empty()) {
        _socketUdp.async_send_to(boost::asio::buffer(_sendQueue.front(), _sendQueue.front().size()), _remoteEndpoint,
            boost::bind(&UdpClient::handlerSend, this,
            boost::asio::placeholders::error));
    } else {
        _writing = 0;
    }
}
