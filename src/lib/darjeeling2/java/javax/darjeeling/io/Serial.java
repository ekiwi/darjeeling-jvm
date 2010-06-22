/*
 * Serial.java
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 
package javax.darjeeling.io;

public class Serial
{
	
	public static native void setBaudRate(byte uartNr, int baudRate);
	public static native byte getNrSerialPorts();
	public static native int getDefaultBaudRate(byte uartNr);
	
	public static SerialInputStream getInputStream(byte uartNr)
	{
		// Check that the uart number is in range.
		if (uartNr<0 || uartNr >= getNrSerialPorts())
			throw new IndexOutOfBoundsException();
		
		return new SerialInputStream(uartNr);
	}
	
	public static SerialOutputStream getOutputStream(byte uartNr)
	{
		// Check that the uart number is in range.
		if (uartNr<0 || uartNr >= getNrSerialPorts())
			throw new IndexOutOfBoundsException();
		
		return new SerialOutputStream(uartNr);
	}
	

}
