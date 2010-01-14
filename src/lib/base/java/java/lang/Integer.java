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
	
	/// Uses characters [0-9a-z] as digits (0 being the smallest, z the largest)
	public static String toString(int i, int base)
	{
		// only supporting base 2 to 36
		if (base < 2 || base > (10+26))
			base = 10;

		// determine number of digits
		int abs = i >= 0 ? i : -i;
		short size = 1;
		while (abs >= base) {
			abs /= base;
			size++;
		}
		
		// reserve space for the '-'
		boolean negative = i < 0;;
		if (negative)
			size++;
		
		// allocate 
		char[] charString = new char[size];

		// convert each digit
		abs = i >= 0 ? i : -i;
		while (size > (negative ? 1 : 0)) {
			byte digit = (byte) (abs % base);
			char ch = (char)(digit < 10 ? '0' + digit :'a' + digit - 10);
			charString[--size] = ch;
			abs /= base;
		}
		
		// prepend '-'
		if (negative)
			charString[0]='-';

		return new String(charString);
		
	}
}
