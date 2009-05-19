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
