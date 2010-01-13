/*
 *	Integer.java
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
package java.lang;

public class Integer
{
	
	private int value;
	
	public Integer(int value) {
		this.value = value;
	}
	
	public int intValue() {
		return value;
	}
	
	public static String toString(int i) {
		// base 10 is default
		return toString(i, 10);
		
	}
	
	public static String toString(int i, int base)
	{
		//TODO: better implementation, maybe native c with atoi?

		// determine number of digits
		int abs = i >= 0 ? i : -i;
		int size = 1;
		while (abs >= base) {
			abs /= base;
			size++;
		}
		
		// reserve space for the '-'
		if (i < 0)
			size++;
		
		// allocate 
		char[] charString = new char[size];

		// convert each digit
		abs = i >= 0 ? i : -i;
		while (size > 0) {
			char ch = (char)('0' + abs % base);
			charString[--size] = ch;
			abs /= base;
		}
		
		// prepend '-'
		if (i < 0)
			charString[0]='-';

		return new String(charString);
		
	}
}
