/* 
*  CHIP-8 Emulator (Interpeter)
*  October 2020
*  ~aml
*/
#pragma once

#include "engine.h"
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <string>

int main(int argc, char* argv[])
{
	if (argc != 4) {
		printf("Invalid arguments. Usage: chip8emulator.exe <game.ch8> <delayValue> <scale>");
		return 0;
	}

	Engine myEngine;
	printf("Loading %s\n", argv[1]);
	myEngine.scale = std::stoi(argv[3]);
	myEngine.loadGame(argv[1]);
	if (!myEngine.initGraphics())
		return 0;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	int cycleDelay = std::stoi(argv[2]);

	while (!myEngine.quit) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float t = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
		if (t > cycleDelay) {
			lastCycleTime = currentTime;
			myEngine.parseInput(myEngine.key);
			myEngine.emulateCycle();
			if (myEngine.drawFlag)
				myEngine.videoRender();
			if (myEngine.delayTimer != 0)
				myEngine.delayTimer--;
		}
	}
	return 0;
}