/*
 * StringBuilder.java
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

public class StringBuilder
{
	private char[] stringStore; //non zero-terminated string
	private int length; //length of the string stored in stringStore
	
	// number of extra chars to reserve in stringStore by growStringStore()
	private static final int GROW_EXTRA=8;

	/// Constructors
	public StringBuilder() {
		stringStore = new char[GROW_EXTRA];
		length = 0;
	}
	
	public StringBuilder append(Object obj) {
		String str = String.valueOf(obj);
		char[] app = str.toCharArray();
		append(app);
		return this;
	}
	
	public StringBuilder append(int integer)
	{
		String str = Integer.toString(integer);
		char[] app = str.toCharArray();
		append(app);
		return this;
	}
	
	public StringBuilder append(boolean b) {
		String str = String.valueOf(b);
		char[] app = str.toCharArray();
		append(app);
		return this;
	}
	
	private void append (char[] app) {
		int appLen = app.length;
		int newLength = length + appLen;
		// grow stringStore when needed
		if (newLength> stringStore.length)
			growStringStore(newLength);
		
		// append string
		System.arraycopy(app, 0, stringStore, length, appLen);

		length = newLength;
	}
	
	public String toString() {
		return new String(stringStore);
	}
	
	private void growStringStore(int size) {
		int newSize=size + GROW_EXTRA;
		// allocate new string store
		char[] newStringStore = new char[newSize];
		// copy old string to new
		System.arraycopy(stringStore, 0, newStringStore, 0, length);
		
		//reference the new array
		stringStore = newStringStore;
	}
}
