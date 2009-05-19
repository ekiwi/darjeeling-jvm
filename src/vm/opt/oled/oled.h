/*
 *	oled.h
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
#ifndef __oled_h__
#define __oled_h__

/*
typedef _fos_oled_color_t fos_oled_color_t;
struct _fos_oled_color_t
{
	UINT16 r:5;
	UINT16 g:6;
	UINT16 b:5;
} __attribute__ ((__packed__));
*/


typedef struct _fos_oled_touchscreen_status_t fos_oled_touchscreen_status_t;
struct _fos_oled_touchscreen_status_t
{
	unsigned char x;
	unsigned int y;
	unsigned char status;
} __attribute__ ((__packed__));

void fos_oled_init();
void fos_oled_cls();
void fos_oled_vsync();

void fos_oled_setFill(char fill);
void fos_oled_setTextOpaque(char opaque);
void fos_oled_setFontSize(int size);
void fos_oled_setBackground(int colour);

void fos_oled_circle(int x, int y, int radius, int colour);
void fos_oled_rectangle(int x1, int y1, int x2, int y2, int colour);
void fos_oled_line(int x1, int y1, int x2, int y2, int colour);
void fos_oled_pixel(int x, int y, int colour);

void fos_oled_copy(int xs, int ys, int xd, int yd, int w, int h);

void fos_oled_putChar(char c, int x, int y, int w, int h, int colour);
void fos_oled_putCharF(char c, int col, int row, int colour);
void fos_oled_putString(char *str, int font, int x, int y, int w, int h, int colour);
void fos_oled_putStringF(char *str, int font, int col, int row, int colour);

unsigned long fos_oled_pollTouchScreen();

#endif
