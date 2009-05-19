/*
 *	TouchScreen.java
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
package javax.oled;

import javax.util.ArrayBag;


public class TouchScreen implements Runnable
{
	
	private static native int _poll();
	
	public static short x,y;
	public static short old_x, old_y;
	public static boolean down;
	private static boolean old_down;

	private static ArrayBag<TouchScreenListener> listeners;
	
	public static void poll()
	{
		int raw = _poll();
		down = ((raw&0xff) == 1); 
		x = (short)((raw>>8) & 0xff); 
		y = (short)( (((raw>>16) & 0xff)<<8) + ((raw>>24) & 0xff) );
		
	}
	
	public static void init()
	{
		listeners = new ArrayBag<TouchScreenListener>();
	}

	public static void start()
	{
		Thread thread = new Thread(new TouchScreen());
		thread.start();
	}
	
	public static void addListener(TouchScreenListener listener)
	{
		listeners.add(listener);
	}
	
	// just here to test
	public static void fireOnDown()
	{
		for (short i=0; i<listeners.size(); i++)
		{
			TouchScreenListener listener = listeners.get(i);
			listener.touchScreenUp(x, y); 
		}
	}		
	
	public void run()
	{
		
		while (true)
		{
			poll();
			
			if (down!=old_down)
			{
				for (short i=0; i<listeners.size(); i++)
				{
					TouchScreenListener listener = listeners.get(i);
					if (down) 
						listener.touchScreenDown(x, y); 
					else 
						listener.touchScreenUp(old_x, old_y);
				}
				
				if (down)
				{
					if ((x>=0)&&(x<240)) old_x = x;
					if ((y>=0)&&(y<320)) old_y = y;
				}
				
			}
			
			old_down = down;

			Thread.sleep(50);
		}
	}

}
