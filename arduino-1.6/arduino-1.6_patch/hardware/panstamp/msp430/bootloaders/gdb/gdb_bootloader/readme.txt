Inspiration:
So I'm looking at making a small msp430fr5739 based board. I was thinking
that I would be able to use the msp430 launchpad to program them. However,
I hit a snag in that they don't seem to recognize the msp430fr5739 as a
valid device. I can use my FRAM boards to program them but not the $10
launchpad. Then there was all the hubbub about the new msp430g2955 chips
and it got me thinking. Why should I let Texas Instruments determine
how I program my chips?

Investigation:
I got to looking at BSL and then got discouraged trying to find a client
that works on windows, linux and OSX. It mostly seemed like it was going
to be a hassle. Then I got to thinking about just using GDB and its
Remote Serial Protocol. If you use msp430-gdb and mspdebug, you use this
interface over a local server socket. However, an often unnoticed feature
of this protocol is that it can also be used with a regular serial port.

Solution:
I decided to implement a gdb stub server that you load once on your msp430
chip. You can think of it as a bootloader that happpens to use the Remote
Serial Protocol. This allows you to load new programs on that chip without
the need for a launchpad or an FET. All you need is a usb->serial port
dongle. You can find these on ebay from $2-$10. They all go faster than
the 9600 baud the virtual serial port the msp430 launchpad provides. It is
likely you probably have one of these already. This scheme doesn't allow
you to debug using msp430-gdb, but it does provide a way to load new code
without having to deal with BSL or having to write any host side code.

How it works:
Load the attached code below on your chip one time. At this point,
you could remove that chip from your launchpad and throw it on a
breadboard. You just need power, the caps and a pull up resistor. Then
connect TX and RX to a serial to USB converter (ftdi, cp102, pl2303hx,
etc..). For simplicity, we can just test with the launchpad itself
and ignore the fact that there is an FET on the board and just use
/dev/ttyACM0 or COM1: on windows.

At reset time, the code looks at the state of the P1.3 button on
the msp430g2553. If you are holding it down, the code starts a gdb
bootload/server running on the msp430g2553. On your linux box, just start
up msp430-gdb to connect to the chip over a serial port. You can't debug,
but what you can do is erase your flash and load a new program. The gdb
server code sits at memory 0xfa00 -> 0xfdff so you do lose some of your
flash. *I've done some optimization see later posts in this thread *


The gdb_bootloader.elf file must be loaded on an msp430g2553. I'll post code
and support for other chips at a later date. I'm excited about this
concept and wanted to let people try it out.

To load the gdb_stub firmware ( do this one time )

	$ mspdebug rf2500 "prog gdb_bootloader.elf"
	MSPDebug version 0.21 - debugging tool for MSP430 MCUs
	Copyright (C) 2009-2012 Daniel Beer <dlbeer@gmail.com>
	This is free software; see the source for copying conditions.  There is NO
	warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	
	Trying to open interface 1 on 059
	rf2500: warning: can't detach kernel driver: No data available
	Initializing FET...
	FET protocol version is 30066536
	Set Vcc: 3000 mV
	Configured for Spy-Bi-Wire
	fet: FET returned error code 4 (Could not find device or device not supported)
	fet: command C_IDENT1 failed
	fet: identify failed
	Trying again...
	Initializing FET...
	FET protocol version is 30066536
	Set Vcc: 3000 mV
	Configured for Spy-Bi-Wire
	Sending reset...
	Device ID: 0x2553
	  Code start address: 0xc000
	  Code size         : 16384 byte = 16 kb
	  RAM  start address: 0x200
	  RAM  end   address: 0x3ff
	  RAM  size         : 512 byte = 0 kb
	Device: MSP430G2553/G2403
	Number of breakpoints: 2
	fet: FET returned NAK
	warning: device does not support power profiling
	Chip ID data: 25 53
	Erasing...
	Programming...
	Writing  884 bytes at fa00 [section: .text]...
	Writing  138 bytes at fd74 [section: .rodata]...
	Writing   32 bytes at ffe0 [section: .vectors]...
	Done, 1054 bytes total

Once you have that installed, you no longer need to use your launchpad
to load new code. To use the gdb bootloader, just connect a usb to serial
dongle to the msp430g2553 directly. Then use a remote serial port to
connect to msp430-gdb. Here are the steps to load a blink program using
msp430-gdb via the serial port:

	$ msp430-gdb -b 9600 -ex 'set remotetimeout 1' -ex 'target remote /dev/ttyACM0' blink.elf 
	GNU gdb (GDB) 7.2
	Copyright (C) 2010 Free Software Foundation, Inc.
	License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
	This is free software: you are free to change and redistribute it.
	There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
	and "show warranty" for details.
	This GDB was configured as "--host=i686-pc-linux-gnu --target=msp430".
	For bug reporting instructions, please see:
	<http://www.gnu.org/software/gdb/bugs/>...
	Reading symbols from /home2/kimballr/github/msp430_code/fabooh/examples/basic/blink/blink.elf...done.
	Remote debugging using /dev/ttyACM0
	_reset_vector__ () at ../../../gcc/gcc/config/msp430/crt0.S:103
	103	../../../gcc/gcc/config/msp430/crt0.S: No such file or directory.
		in ../../../gcc/gcc/config/msp430/crt0.S
	(gdb) erase
	Erasing all flash memory
	ok
	(gdb) load
	Loading section .text, size 0x74 lma 0xc000
	Loading section .vectors, size 0x20 lma 0xffe0
	Start address 0xc000, load size 148
	Transfer rate: 264 bytes/sec, 16 bytes/write.
	(gdb) quit
	A debugging session is active.
	Inferior 1 [Remote target] will be killed.
	Quit anyway? (y or n) y 

At this point, there are two things on your msp430g2553 chip. At 0xfa00 is
the gdb_bootloader code, it is run first before your blink.elf code. It
checks the button at reset to see if you are holding it down. If not,
then it runs the blink.elf code. If you are holding it down, it runs the
gdb stub server and allows you to reload code using msp430-gdb.

So just press reset to start the blink.

Press and hold the P1.3 switch down then press reset to start the gdb
stub server. The RED led comes on solid if it is ready to accept a
serial gdb connection.

Pretty simple huh? I think so.

The code is written in C++ using my fabooh framework. I've been trying
to find a good app to use as the fabooh debut project. I think this will
be it. This type of code needs to be small but needs the flexibility to
use various UART implementations. This code personifies what fabooh does
well. So expect to see it real soon now, in the fabooh source tree.

-rick


