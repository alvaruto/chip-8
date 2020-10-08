#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS //Required flag to use fopen on Windows
#endif

#include "engine.h"

Engine::Engine() {
	pc = 0x200;
	//Load fontset
	for (int i = 0; i < 80; ++i)
		memory[0x50 + i] = chip8_fontset[i];
}

Engine::~Engine() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Engine::initGraphics() {
	int SCREEN_WIDTH = (int)(64 * scale);
	int SCREEN_HEIGHT = (int)(32 * scale);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else {
		window = SDL_CreateWindow("chip8emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_RenderSetScale(renderer, scale, scale);
		}

	}
	return true;
}

void Engine::loadGame(const char* game) {
	FILE* f = fopen(game, "rb");
	if (f == NULL)
		printf("Error: Couldn't open %s. %s\n", game, strerror(errno));
	else
		printf("%s opened\n", game);

	fseek(f, 0L, SEEK_END); //Seek end of file
	int romsize = ftell(f); //Size = last position
	fseek(f, 0L, SEEK_SET); //Seek beginning of file
	char* buffer = new char[romsize];
	fread(buffer, romsize, 1, f);

	for (long i = 0; i < romsize; ++i) {
		memory[512 + i] = buffer[i];
	}
}

void Engine::parseInput(unsigned char* key) {
	SDL_Event input;

	while (SDL_PollEvent(&input)) {
		switch (input.type) {
		case SDL_QUIT:
		{
			quit = true;
		} break;

		case SDL_KEYDOWN:
		{
			switch (input.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				quit = true;
			} break;

			case SDLK_x:
			{
				key[0] = 1;
			} break;

			case SDLK_1:
			{
				key[1] = 1;
			} break;

			case SDLK_2:
			{
				key[2] = 1;
			} break;

			case SDLK_3:
			{
				key[3] = 1;
			} break;

			case SDLK_q:
			{
				key[4] = 1;
			} break;

			case SDLK_w:
			{
				key[5] = 1;
			} break;

			case SDLK_e:
			{
				key[6] = 1;
			} break;

			case SDLK_a:
			{
				key[7] = 1;
			} break;

			case SDLK_s:
			{
				key[8] = 1;
			} break;

			case SDLK_d:
			{
				key[9] = 1;
			} break;

			case SDLK_z:
			{
				key[0xA] = 1;
			} break;

			case SDLK_c:
			{
				key[0xB] = 1;
			} break;

			case SDLK_4:
			{
				key[0xC] = 1;
			} break;

			case SDLK_r:
			{
				key[0xD] = 1;
			} break;

			case SDLK_f:
			{
				key[0xE] = 1;
			} break;

			case SDLK_v:
			{
				key[0xF] = 1;
			} break;
			}
		} break;

		case SDL_KEYUP:
		{
			switch (input.key.keysym.sym) {
			case SDLK_x:
			{
				key[0] = 0;
			} break;

			case SDLK_1:
			{
				key[1] = 0;
			} break;

			case SDLK_2:
			{
				key[2] = 0;
			} break;

			case SDLK_3:
			{
				key[3] = 0;
			} break;

			case SDLK_q:
			{
				key[4] = 0;
			} break;

			case SDLK_w:
			{
				key[5] = 0;
			} break;

			case SDLK_e:
			{
				key[6] = 0;
			} break;

			case SDLK_a:
			{
				key[7] = 0;
			} break;

			case SDLK_s:
			{
				key[8] = 0;
			} break;

			case SDLK_d:
			{
				key[9] = 0;
			} break;

			case SDLK_z:
			{
				key[0xA] = 0;
			} break;

			case SDLK_c:
			{
				key[0xB] = 0;
			} break;

			case SDLK_4:
			{
				key[0xC] = 0;
			} break;

			case SDLK_r:
			{
				key[0xD] = 0;
			} break;

			case SDLK_f:
			{
				key[0xE] = 0;
			} break;

			case SDLK_v:
			{
				key[0xF] = 0;
			} break;
			}
		} break;
		}
	}

}

void Engine::videoRender() {
	//Blank the screen with black before composing the frame
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);

	//Compose the frame with white
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int rowNum;

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			rowNum = y * 64;
			if (gfx[x + rowNum] != 0)
				SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	SDL_RenderPresent(renderer);
	drawFlag = false;
}

void Engine::emulateCycle() {
	//Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	pc += 2; //Opcode has already been fetched, advance to next instruction for next cycle.

	//Decode and execute opcode
	switch (opcode & 0xF000) {
	case 0x0000:
		switch (opcode) {
		case 0x00E0: //Clear graphics
			std::fill(std::begin(gfx), std::end(gfx), 0);
			break;
		case  0x00EE: //Return from a subrutine
			sp--;
			pc = stack[sp];
			break;
		}
		break;
	case 0x1000: //Jump to address 0x0NNN
		pc = opcode & 0x0FFF;
		break;
	case 0x2000: //Execute subrutine starting at 0x0NNN
		stack[sp] = pc;
		sp++;
		pc = opcode & 0x0FFF;
		break;
	case 0x3000: //Skip the following instruction if the value of register VX equals NN
		if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
			pc += 2;
		break;
	case 0x4000: //Skip the following instruction if the value of register VX is not equal to NN
		if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
			pc += 2;
		break;
	case 0x5000: //Skip the following instruction if the value of register VX is equal to the value of register VY
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			pc += 2;
		break;
	case 0x6000: //Store number NN in register VX
		V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		break;
	case 0x7000: //Add the value NN to register VX
		V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
		break;
	case 0x8000:
		switch (opcode & 0xF00F) {
		case 0x8000: //Store the value of register VY in register VX
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			break;
		case 0x8001: //Set VX to VX OR VY
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
			break;
		case 0x8002: //Set VX to VX AND VY
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
			break;
		case 0x8003: //Set VX to VX XOR VY
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
			break;
		case 0x8004: //Add the value of register VY to register VX
			if ((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 0xFF)
				V[15] = 1;
			else
				V[15] = 0;
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			break;
		case 0x8005: //Subtract the value of register VY from register VX (VX - VY)
			if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
				V[15] = 1;
			else
			{
				V[15] = 0;
			}
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			break;
		case 0x8006: //Store the value of register VY shifted right one bit in register VX
			V[15] = (V[(opcode & 0x00F0) >> 4] & 0x01);
			V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] >> 1);
			break;
		case 0x8007: //Set register VX to the value of VY minus VX
			if (V[(opcode & 0x00F0) >> 4] >= V[(opcode & 0x0F00) >> 8])
				V[15] = 1;
			else
				V[15] = 0;
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
			break;
		case 0x800E: //Store the value of register VY shifted left one bit in register VX
			V[15] = V[(opcode & 0x00F0) >> 4] >> 7; //Most significant bit is stored in VF before the shift.
			V[15] = V[(opcode & 0x00F0) >> 4] >> 7; //Most significant bit is stored in VF before the shift.
			V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] << 1);
			break;
		}
		break;
	case 0x9000: //Skip the following instruction if the value of register VX is not equal to the value of register VY
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
			pc += 2;
		else	 
		break;
	case 0xA000: //Store memory address NNN in register I
		I = opcode & 0x0FFF;
		break;
	case 0xB000: //Jump to address NNN + V0
		pc = (opcode & 0x0FFF) + V[0];
		break;
	case 0xC000: //Set VX to a random number with a mask of NN
		V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x0FF);
		break;
	case 0xD000: //Draw a sprite (...)
	{
		unsigned short x = V[(opcode & 0x0F00) >> 8];
		unsigned short y = V[(opcode & 0x00F0) >> 4];
		unsigned short height = opcode & 0x000F;
		unsigned short pixel;
		//Each sprite has a fixed width (8 bits) and a variable height.
		V[15] = 0;
		for (int yline = 0; yline < height; yline++)
		{
			pixel = memory[I + yline];
			for (int xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gfx[(x + xline + (y + yline) * 64)] == 1)
						V[15] = 1;
					gfx[x + xline + ((y + yline) * 64)] ^= 1;
				}
			}
		}

		drawFlag = true;
		break;
	}
	case 0xE000: //Skip following instruction if the key (...)
		if ((opcode & 0x00FF) == 0x09E) {
			unsigned char x = (opcode & 0x0F00) >> 8;
			if (key[V[x]])
				pc += 2;
		}
		else {
			unsigned char x = (opcode & 0x0F00) >> 8;
			if (!key[V[x]])
				pc += 2;
		}
		break;
	case 0xF000:
		uint8_t x{};
		uint8_t value{};
		switch (opcode & 0x0FF) {
		case 0x07: //Store the current value of the delay timer in register VX
			x = (opcode & 0x0F00) >> 8;
			V[x] = delayTimer;
			break;
		case 0x0A: //Wait for a keypress and store the result in register VX
			printf("0xFX0A is unimplemented\n");
			break;
		case 0x015: //Set the delay timer to the value of register VX
			x = (opcode & 0x0F00) >> 8;
			delayTimer = V[x];
			break;
		case 0x018: //Set the sound timer to the value of register VX
			x = (opcode & 0x0F00) >> 8;
			soundTimer = V[x];
			break;
		case 0x01E: //Add the value stored in register VX to register I
			x = (opcode & 0x0F00) >> 8;
			I += V[x];
			break;
		case 0x029: //Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
			x = (opcode & 0x0F00) >> 8;
			I = 0x50 + (5 * V[x]);
			break;
		case 0x033: //Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
			x = (opcode & 0x0F00) >> 8;
			value = V[x];
			// Ones-place
			memory[I + 2] = value % 10;
			value /= 10;
			// Tens-place
			memory[I + 1] = value % 10;
			value /= 10;
			// Hundreds-place
			memory[I] = value % 10;
			break;
		case 0x055: //Store the values of registers V0 to VX inclusive in memory starting at address I. I is set to I + X + 1 after operation
			x = (opcode & 0x0F00) >> 8;
			for (uint8_t i = 0; i <= x; i++) {
				memory[I + i] = V[i];
			}
			I += x + 1;
			break;
		case 0x065: //Fill registers V0 to VX inclusive with the values stored in memory starting at address I. I is set to I + X + 1 after operation
			x = (opcode & 0x0F00) >> 8;
			for (uint8_t i = 0; i <= x; i++) {
				V[i] = memory[I + i];
			}
			I += x + 1;
			break;
		}
		break;
	}
}
