/*
 * PacketLengthExceededException.java
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
 
package javax.darjeeling.radio;

/**
 * Thrown when the application tries to send a packet that is larger than the maximum packet length.
 * 
 * @author Niels Brouwers
 *
 */
public class PacketLengthExceededException extends RuntimeException
{
	
	public PacketLengthExceededException()
	{
	}
	
	public String toString()
	{
		return "PacketLengthExceededException";
	}

}
