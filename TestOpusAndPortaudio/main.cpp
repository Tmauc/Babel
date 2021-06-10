/*
** EPITECH PROJECT, 2019
** main
** File description:
** main
*/

#include "PortAudio.hpp"
#include "Opus.hpp"

int main(int argc, char *argv[])
{
    PortAudio pa;
    
    std::cerr << "PortAudio was successfully initialized" << std::endl;

    Opus o;

    std::cerr << "Codec initialized" << std::endl;

    if (!pa.createStreams(o._encoder, o._decoder)) {
        std::cerr << "I can not create I / O streams" << std::endl;
        return 1;
    }

    if (!pa.startStreams()) {
        std::cerr << "I can not start I / O streams" << std::endl;
        return 1;
    }

    std::cerr << "" << std::endl;
    std::cerr << "The audio codec test is started ..." << std::endl;
    std::cerr << "To complete, type q" << std::endl;

    char q;
    for(int i = 30; i != 0; --i) {
        std::cin >> q;
        if ( q == 'q' ) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::cerr << "" << std::endl;
    std::cerr << "Test complete" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    std::cerr << "Ending the program ..." << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    pa.stopStreams();
    return 0;
}
