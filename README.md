# Raspboot
Raspboot is a light-weight utility for loading binaries and interacting with memory on the Raspbery Pi. All communication is currently done over the serial port (UART) to keep the application small. I plan to add support for communicating over Ethernet at a later point, however my focus is on completing the core featues needed for my own development. (See the TODO list below)

Raspboot is intended for bare metal development on the Pi. It won't be able to load a Linux kernel, for example, nor would there be any benefit to doing so even if it could. The main reason to use Raspboot is that eliminates the need to reflash the Pi's SD card each time a code change is made. Raspboot can be used to transfer the program binary over a serial connection and load it directly into the Pi's memory, shortening turn-around times when testing code. A variety of other useful commands are planned, such as memory and register dumps.

The project is currently in its infancy; the version currently in the repository can only be used to upload binaries to the device. The interactive mode for entering commands is still incomplete, however I plan to commit it by the end of the week.

## Client/Server model
Raspboot uses a client/server model. The Raspberry Pi runs a server application and a Linux machine runs the client. Commands are entered via the client application, which sends them over a serial connection to the server. The server performs the command and sends back a reply if neccessary.

## Compiling
To compile code for the Raspberry Pi, the system needs to have the `arm-none-eabi` toolchain installed. On Debian/Mint/Ubuntu this can be installed via the package manager, e.g. `apt-get install gcc-arm-none-eabi`.

Raspboot can be compiled by running `make` in the checked out directory. The Makefile will build both the client (`bin/raspboot`) and server (`bin/raspboot-server`) applications. Running `make install` will install the Raspboot client binary to `/usr/local/bin`. 

A Doxygen configuration file is included in the root directory of the project. Running `make doc` will compile the in-source documentation to the `./doc` directory. `make clean_doc` can be used to delete the documentation files.

## Usage
The server binary needs to be written to the Raspberry Pi's SD card and set as the bootable image. To do so, copy the server application to the boot partition of the SD card and add `kernel=raspboot-server` to the end of the `config.txt` file in the same directory. Once this is done, the card can be reinserted in the Raspberry Pi and the device powered on.

To transfer a binary with Raspboot:
```
raspboot -b /path/to/binary.bin -l 0x16000 /dev/ttyUSB0
```
The `-b` argument indicates which binary file to tranfer, `-l` is the location in the Pi's memory to upload the binary and the final argument is the serial port to open a connection to.

The current client runs in a loop, so if the uploaded binary terminates and returns out properly, it should jump back into the Raspboot server. Another binary can then be uploaded using the same command. Note that Raspboot will stack registers `r0` through `r12` and the `link register (r14)` before branching to the uploaded code.

## Command Line Arguments
Raspboot can be configured with the following arguments, all of which are optional:
```
-b      Path to the binary file to upload [default: none]
-g      Jump into the uploaded binary as soon as the upload finishes [default: disabled]
-t      Terminate Raspboot after running the binary (typically used with -g) [default: disabled]
-h      Print usage information if no other args are specified
-l      The memory location to write the binary to [default: 0x10000]
-r      The baud rate, as listed in the termios docs, e.g. 4800, 9600, 115200 [default: 115200]
-v      The verbosity level. Errors = 0, Warnings = 1, General Info = 2. [default: 2]
```
## Interactive Mode Commands
If the `-t` command is not specified, Rasboot will enter interactive mode after uploading the binary file (if any). The following commands are available (optional parameters listed in [square brackets]):
```
get -l loc [-c count]   Reads and prints 'count' bytes from 'loc'. 'count' defaults to 4 bytes.
go [-l loc]             Executes code at 'loc'. Defaults to the value passed over the command line.
help                    Lists available commands
quit                    Exits Raspboot
```
Arguments can be passed in any order, e.g. both `get -l 0x10000 -c 12` and `get -c 12 -l 0x10000` are valid.

## Todo List
* ~~Properly use the speed/baud rate parameter~~
* ~~Add verbosity settings, especially for interactive mode~~
* Optionally pass parameters to the binary when executing it
* Finish interactive mode 
* Switch to using `getopt` for interactive mode command parsing
* ~~Finish documentation~~
* ~~Modularize server-side UART code~~
* Add Ethernet support (long term)
* GitHub Wiki documentation (long term)

## Other Platforms
It should be relatively straightforward to port the device server to a different platform. The only portion of the code that is specific to the Raspberry Pi is the serial port interface.

## License
Both the Raspboot server and client are distributed under [The MIT License](https://opensource.org/licenses/MIT).
