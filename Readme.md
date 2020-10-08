# CHIP-8 Emulator (Interpeter)

Very basic CHIP-8 emulator written in C/C++ using Simple DirectMedia Layer as input/graphics library.

## Usage
> chip8emulator.exe game.ch8 delayValue scale

Where delayValue will vary between one computer and another (this is because the CHIP-8 clock is adjusted using a busy loop, will change in the future), 
but a natural number between 1 and 10 should be fine.
scale is up to the user, 10 or 20 are good numbers to try for the first time.

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
	
## Where can I download games to try it?
I have played some games as well as tested the emulator implementation using ROMs from [The Old Computer](https://www.theoldcomputer.com/roms/index.php?folder=Chip-8/Chip-8/Test-Programs)

## References
*	[Building a CHIP-8 Emulator [C++]](https://austinmorlan.com/posts/chip8_emulator/)
*	[How to write an emulator (CHIP-8 interpreter)](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
*	[Mastering CHIP-8 by Matthew Mikolay](http://mattmik.com/files/chip8/mastering/chip8.html)
*	[SDL-CHIP-8](https://github.com/swstegall/SDL-CHIP8)