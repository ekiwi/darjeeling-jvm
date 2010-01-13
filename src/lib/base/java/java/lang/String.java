/*
 *	String.java
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

public class String
{
	/* IMPORTANT: these three variables should be at the beginning of the String class.
	 * Do not change their type or order! 
	 * Darjeelings LDS instruction expects them to be in this order.
	 */
	private char[] stringStore;// contains an immutable, unterminated string.
	private int offset;
	private int stringLength;
	/* More variables can be added AFTER this comment statement. */
	
	public String() {
		stringStore = new char[0];
		offset = 0;
		stringLength = 0;
	}
	
	public String(String original) {
		stringStore = original.stringStore;
		offset = original.offset;
		stringLength = original.stringLength;
	}

	public String(char[] charStr) {
		if (charStr == null)
			throw new NullPointerException();
		
		// find length of the string contained in charStr
		stringLength = charStr.length;
		for (int i=0;i<charStr.length;i++) {
			if (charStr[i] == 0){
				stringLength = i;
				break;
			}
		}
		// allocate memory
		stringStore = new char[stringLength];
		// copy 
		System.arraycopy(charStr, 0, stringStore, 0, stringLength);

		offset = 0;
	}
	
	public String(byte[] byteStr) {
		if (byteStr == null)
			throw new NullPointerException();
		
		// find length of the string contained in byteStr
		stringLength = byteStr.length;
		for (int i=0;i<byteStr.length;i++) {
			if (byteStr[i] == 0){
				stringLength = i;
				break;
			}
		}
		// allocate memory
		stringStore = new char[stringLength];
		// copy 
		for (int i=0; i<stringLength;i++)
			stringStore[i] = (char)byteStr[i];

		offset = 0;
	}
	
	public char charAt(int index) {
		if (index < 0 || index >= stringLength)
			throw new IndexOutOfBoundsException();
		return stringStore[index];
	}
	
	public boolean equals(String str) {
		if (str == null)
			return false;
		if (this.stringLength != str.stringLength)
			return false;
		
		int remaining = this.stringLength;
		int i1=this.offset,i2=str.offset;
		char ch1, ch2;
		
		// look for unequal chars in the strings
		while (remaining-- > 0) {
			ch1 = this.stringStore[i1++];
			ch2 = str.stringStore[i2++];
			if (ch1 != ch2)
				return false;
		}
		return true;
	}

	public int length() {
		return stringLength;
	}
	
	public String substring(int beginIndex) {
		return substring(beginIndex, stringLength);
	}
	
	public String substring(int beginIndex, int endIndex) {
		if (beginIndex < 0 || beginIndex >= stringLength || endIndex <= beginIndex || endIndex > stringLength)
			throw new IndexOutOfBoundsException();
		
		/* copy string. The strings will reference the same stringStore.
		 * This can improve memory usage, but also prevents deallocation
		 * of the original (bigger) stringStore when in fact only a small
		 * part is needed. 
		 */
		String ret = new String(this);
		// set boundaries
		ret.offset += beginIndex;
		ret.stringLength = endIndex-beginIndex;
		return ret;
	}
	
	public String toString() {
		return this;
	}
	
	public byte[] toByteArray() {
		byte[] ret = new byte[stringLength];
		for (int i=0; i<stringLength; i++)
			ret[i] = (byte)stringStore[offset+i];
		return ret;
	}
	
	public byte[] toZeroTerminatedByteArray() {
		byte[] ret = new byte[stringLength+1];
		for (int i=0; i<stringLength; i++)
			ret[i] = (byte)stringStore[offset+i];
		ret[stringLength] = '\0';
		return ret;
	}
	
	public char[] toCharArray() {
		char[] ret = new char[stringLength];
		System.arraycopy(stringStore, offset, ret, 0, stringLength);
		return ret;
	}
	
	public static String valueOf(Object obj ){
		if (obj == null)
			return "null";
		return obj.toString();
	}
	
	public static String valueOf(char[] charArray ){
		if (charArray == null)
			return "null";
		
		return new String(charArray);
	}
	
	public static String valueOf(byte[] byteArray ){
		if (byteArray == null)
			return "null";
		
		return new String(byteArray);
	}
	
	public static String valueOf(boolean b ){
		if (b)
			return "true";
		else
			return "false";
	}
	
	public static String valueOf(int i ){
		return Integer.toString(i);
	}
}
