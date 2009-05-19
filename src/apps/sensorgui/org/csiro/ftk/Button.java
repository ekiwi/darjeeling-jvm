package org.csiro.ftk;
import javax.oled.Screen;
import javax.util.ArrayBag;



public class Button extends Widget
{
	
	private boolean down;
	private String text;
	protected ArrayBag<ActionListener> actionListeners;
	
	public Button(short x, short y, short w,short h, String text)
	{
		super(x,y,w,h);
		this.text = text;
		this.actionListeners = new ArrayBag<ActionListener>();
	}
	
	public void addActionHandler(ActionListener listener)
	{
		this.actionListeners.add(listener);
	}

	public void draw(short x, short y, short w, short h)
	{
		
		if (down)
		{
			Screen.setFill(false);
			Screen.rectangle(x+1,y+1,x+w-2,y+h-2,backgroundColor);
			Screen.setFill(true);
			Screen.rectangle(x+2,y+2,x+w-3,y+h-3,foregroundColor);
		} else
		{
			Screen.setFill(true);
			Screen.rectangle(x+1,y+1,x+w-2,y+h-2,backgroundColor);
		}
		
		Screen.setFill(false);
		Screen.rectangle(x,y,x+w-1,y+h-1,foregroundColor);
		
		// draw button text
		Screen.putCenteredString(text,1, x, y, w, h, down?backgroundColor:foregroundColor);		
		
	}
	
	public boolean onDown(short x, short y)
	{
		down = true;
		makeDirty();
		return true;
	}
	
	public boolean onUp(short x, short y)
	{
		down = false;
		makeDirty();
		
		// call action handlers
		for (short i=0; i<actionListeners.size(); i++)
			actionListeners.get(i).action(this);
		
		
		return true;
	}

}
