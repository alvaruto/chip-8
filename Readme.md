# CHIP-8 Emulator (Interpeter)

Very basic CHIP-8 emulator written in C/C++ using Simple DirectMedia Layer as input/graphics library.

## Usage
> chip8emulator.exe <game.ch8> <delayValue> <scale>
Where delayValue will vary between one computer and another (this is because the CHIP-8 clock is adjusted using a busy loop, will change in the future), 
but a natural number between 1 and 10 should be fine.
scale is up to the user, 10 or 20 are good numbers to try at first.

Hex keypad is setup like this:

	Keypad 		Keyboard
	+-+-+-+-+ 	+-+-+-+-+
	|1|2|3|C| 	|1|2|3|4|
	+-+-+-+-+ 	+-+-+-+-+
	|4|5|6|D| 	|Q|W|E|R|
	+-+-+-+-+	+-+-+-+-+
	|7|8|9|E| 	|A|S|D|F|
	+-+-+-+-+ 	+-+-+-+-+
	|A|0|B|F| 	|Z|X|C|V|
	+-+-+-+-+ 	+-+-+-+-+