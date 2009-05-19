/*
 *	nesc.h
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
#ifndef __nesc_h__
#define __nesc_h__

int nesc_printf(char * msg);
uint32_t nesc_getTime();
void nesc_setLed(int nr, int on);

uint16_t nesc_getMaxPayloadLength();
int nesc_send(const char * message, int16_t receiverId, uint16_t length);
int nesc_wasAcked();

uint16_t nesc_peekMessageLength();
void * nesc_popMessageBuffer();
int nesc_getNrMessages();

#endif
