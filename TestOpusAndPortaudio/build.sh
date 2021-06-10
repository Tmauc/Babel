#!/bin/bash

g++ -o TestOpusPortAudio main.cpp PortAudio.cpp Opus.cpp -lopus -lportaudio -lpthread
