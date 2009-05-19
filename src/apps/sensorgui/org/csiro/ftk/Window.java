package org.csiro.ftk;
import javax.oled.TouchScreen;
import javax.oled.TouchScreenListener;

public class Window extends Panel implements TouchScreenListener
{

	public Window(short x, short y, short w, short h)
	{
		super(x, y, w, h);
		showBorder = false;
		TouchScreen.addListener(this);
	}
	
	public void touchScreenUp(short x, short y)
	{
		synchronized(this)
		{
			if (onUpRec(x, y))
				update();
		}
	}

	public void touchScreenDown(short x, short y)
	{
		synchronized(this)
		{
			if (onDownRec(x, y))
				update();
		}
	}
	
	public void update()
	{
		synchronized(this)
		{
			drawRec((short)0,(short)0,false);
		}
	}

}
