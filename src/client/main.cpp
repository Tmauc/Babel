/*
** EPITECH PROJECT, 2019
** babel main client
** File description:
** babel
*/

#include "BabelWindow.hpp"
#include "BabelClient.hpp"

int main(int ac, char **av)
{
    QApplication a(ac, av);
    BabelWindow w;

    w.show();
/*	try {
		boost::asio::io_service io_service;
		UdpClient client(io_service);
		io_service.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}*/
    return a.exec();
}
