/*
** EPITECH PROJECT, 2019
** babel server
** File description:
** server
*/

#include <ctime>
#include <iostream>
#include <string>
#include "Server.hpp"

int main(void)
{
    Acceptor accept;
    Server oServ(&accept);
    accept.run();
}
