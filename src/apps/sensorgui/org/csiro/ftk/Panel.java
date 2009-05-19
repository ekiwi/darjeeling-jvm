package org.csiro.ftk;
import javax.oled.Screen;


public class Panel extends Widget
{
	
	protected boolean showBorder = false;

	public Panel(short x, short y, short w, short h)
	{
		super(x, y, w, h);
	}

	public void setShowBorder(boolean showBorder)
	{
		this.showBorder = showBorder;
	}
	
	public void draw(short x, short y, short w, short h)
	{
		
		if (showBorder)
		{
			Screen.setFill(true);
			Screen.rectangle(x+1,y+1,x+w-2,y+h-2,backgroundColor);
			Screen.setFill(false);
			Screen.rectangle(x,y,x+w-1,y+h-1,foregroundColor);
		} else
		{
			Screen.setFill(true);
			Screen.rectangle(x,y,x+w-1,y+h-1,backgroundColor);
		}
	}	

}
