/*
 *	oled.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "oled.h"
#include "fos/rtc.h"
#include "fos/serial.h"

#include "stdio.h"
#include "string.h"

#define FOS_PORT 1

static inline void fos_oled_sendChar(unsigned char c)
{
	fos_serial_write(FOS_PORT, &c, 1);
}

static inline void fos_oled_sendString(char *str)
{
	fos_serial_write(FOS_PORT, (unsigned char*)str, strlen(str)+1);
}

static inline unsigned char fos_oled_getChar()
{
	unsigned char c;
	while (fos_serial_read(FOS_PORT, &c, 1)==0);
	return c;
}

static inline void fos_oled_waitAck()
{
	fos_oled_getChar();
}

static inline void fos_oled_sendInt16(int v)
{
	fos_oled_sendChar(v >> 8);
	fos_oled_sendChar(v);
}

static inline void fos_oled_sendInt8(int v)
{
	fos_oled_sendChar(v);
}

static inline void fos_oled_sendColour(int v)
{
	fos_oled_sendInt16(v);
}

void fos_oled_init()
{
	// send 'U' command, autobaud
	fos_oled_sendChar('U');

	// get response
	fos_oled_waitAck();
}

void fos_oled_cls()
{
	// send 'E' command, clear screen
	fos_oled_sendChar('E');
	fos_oled_waitAck();
}


void fos_oled_vsync()
{
	fos_oled_sendChar('$');
	fos_oled_sendChar('V');
	fos_oled_waitAck();
}


void fos_oled_setFill(char fill)
{
	fos_oled_sendChar('p');
	fos_oled_sendInt8(fill^1);
	fos_oled_waitAck();
}

void fos_oled_setTextOpaque(char opaque)
{
	fos_oled_sendChar('O');
	fos_oled_sendInt8(opaque);
	fos_oled_waitAck();
}

void fos_oled_setFontSize(int size)
{
	fos_oled_sendChar('F');
	fos_oled_sendInt8(size);
	fos_oled_waitAck();
}

void fos_oled_setBackground(int colour)
{
	fos_oled_sendChar('B');
	fos_oled_sendColour(colour);
	fos_oled_waitAck();
}


void fos_oled_circle(int x, int y, int radius, int colour)
{
	fos_oled_sendChar('C');

	fos_oled_sendInt8(x);
	fos_oled_sendInt16(y);
	fos_oled_sendInt8(radius);
	fos_oled_sendColour(colour);

	fos_oled_waitAck();
}

void fos_oled_rectangle(int x1, int y1, int x2, int y2, int colour)
{
	fos_oled_sendChar('r');

	fos_oled_sendInt8(x1);
	fos_oled_sendInt16(y1);
	fos_oled_sendInt8(x2);
	fos_oled_sendInt16(y2);
	fos_oled_sendColour(colour);

	fos_oled_waitAck();
}

void fos_oled_line(int x1, int y1, int x2, int y2, int colour)
{
	fos_oled_sendChar('L');

	fos_oled_sendInt8(x1);
	fos_oled_sendInt16(y1);
	fos_oled_sendInt8(x2);
	fos_oled_sendInt16(y2);
	fos_oled_sendColour(colour);

	fos_oled_waitAck();
}

void fos_oled_pixel(int x, int y, int colour)
{
	fos_oled_sendChar('P');

	fos_oled_sendInt8(x);
	fos_oled_sendInt16(y);
	fos_oled_sendColour(colour);

	fos_oled_waitAck();
}

void fos_oled_copy(int xs, int ys, int xd, int yd, int w, int h)
{
	fos_oled_sendChar('c');

	fos_oled_sendInt8(xs);
	fos_oled_sendInt16(ys);
	fos_oled_sendInt8(xd);
	fos_oled_sendInt16(yd);
	fos_oled_sendInt8(w);
	fos_oled_sendInt16(h);

	fos_oled_waitAck();
}

void fos_oled_putChar(char c, int x, int y, int w, int h, int colour)
{
	fos_oled_sendChar('t');

	fos_oled_sendChar(c);
	fos_oled_sendInt8(x);
	fos_oled_sendInt16(y);
	fos_oled_sendColour(colour);
	fos_oled_sendInt8(w);
	fos_oled_sendInt8(h);

	fos_oled_waitAck();
}

void fos_oled_putCharF(char c, int col, int row, int colour)
{
	fos_oled_sendChar('T');

	fos_oled_sendChar(c);
	fos_oled_sendInt8(col);
	fos_oled_sendInt8(row);
	fos_oled_sendColour(colour);

	fos_oled_waitAck();
}

void fos_oled_putString(char *str, int font, int x, int y, int w, int h, int colour)
{
	fos_oled_sendChar('S');

	fos_oled_sendInt8(x);
	fos_oled_sendInt16(y);
	fos_oled_sendInt8(font);
	fos_oled_sendColour(colour);
	fos_oled_sendInt8(w);
	fos_oled_sendInt8(h);

	fos_oled_sendString(str);

	fos_oled_waitAck();
}

void fos_oled_putStringF(char *str, int font, int col, int row, int colour)
{
	fos_oled_sendChar('s');

	fos_oled_sendInt8(col);
	fos_oled_sendInt8(row);
	fos_oled_sendInt8(font);
	fos_oled_sendColour(colour);

	fos_oled_sendString(str);

	fos_oled_waitAck();
}

/*
fos_oled_touchscreen_status_t fos_oled_pollTouchScreen()
{
	unsigned char buf[4];
	fos_oled_touchscreen_status_t ret;
	fos_oled_sendChar('&');
	fos_oled_sendChar('T');

	// get for response
	fos_serial_read(0, buf, 4);

	ret.status = buf[0];
	ret.x = buf[1];
	ret.y = (buf[2] << 8) + buf[3];

	return ret;
}
*/


unsigned long fos_oled_pollTouchScreen()
{

	unsigned char cmd[2] = { '$', 'T' };
	unsigned long ret;

	fos_serial_write(FOS_PORT, cmd, 2);

	// get for response
	fos_serial_read(FOS_PORT, (unsigned char*)(&ret), 4);

	return ret;
}
