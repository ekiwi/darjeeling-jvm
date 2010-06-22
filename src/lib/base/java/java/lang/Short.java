/*
 * Short.java
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
 
package java.lang;

public class Short
{
	
	private short value;
	
	public Short(short value)
	{
		this.value = value;
	}
	
	public static Short valueOf(short s)
	{
		return new Short(s);
	}
	
	public static String toString(short i)
	{
		// base 10 is default
		return Integer.toString((int)i, 10);
	}

	public static String toString(short i, int base)
	{
		return Integer.toString((int)i, base);
	}

	public short shortValue()
	{
		return value;
	}
	
}
