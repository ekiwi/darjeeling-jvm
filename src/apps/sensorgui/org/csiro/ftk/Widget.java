package org.csiro.ftk;
import javax.util.ArrayBag;

public class Widget
{
	
	public short x,y,w,h;
	protected ArrayBag<Widget> children;
	protected short backgroundColor = (short)0x0;
	protected short foregroundColor = (short)0xffff;
	protected boolean dirty = true;
	protected boolean visible = true;
	
	public Widget(short x, short y, short w,short h)
	{
		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
		this.children = new ArrayBag<Widget>((short)4);
	}
	
	public void addChild(Widget w)
	{
		this.children.add(w);
		makeDirty();
	}

	public void drawRec(short tx, short ty, boolean force)
	{
		if (!visible) return;

		if (this.dirty||force)
			draw((short)(tx + x), (short)(ty + y), w, h);
		
		for (short i=0; i<children.size(); i++)
		{
			Widget w = (Widget)children.get(i); 
			w.drawRec((short)(tx + x), (short)(ty + y), this.dirty||force );
		}
		
		this.dirty = false;
	}
	
	protected boolean inside(short x, short y)
	{
		return (x>=this.x)&&(x<this.x+this.w)&&(y>=this.y)&&(y<this.y+this.h);
	}
	
	public boolean onDownRec(short x, short y)
	{
		if (inside(x,y)&&visible)
		{
			short wx = (short)(x - this.x);
			short wy = (short)(y - this.y);
			for (short i=0; i<children.size(); i++)
			{
				Widget w = (Widget)children.get(i);
				if (w.inside(wx, wy) && w.onDownRec(wx, wy)) return true;
			}
			return onDown(wx, wy);
		} else
			return false;
	}
	
	public boolean onUpRec(short x, short y)
	{
		if (inside(x,y)&&visible)
		{
			short wx = (short)(x - this.x);
			short wy = (short)(y - this.y);
			for (short i=0; i<children.size(); i++)
			{
				Widget w = (Widget)children.get(i);
				if (w.inside(wx, wy) && w.onUpRec(wx, wy)) return true;
			}
			return onUp(wx, wy);
		} else
			return false;
	}
	
	protected void makeDirty()
	{
		dirty = true;
	}
	
	public void setBackgroundColor(short backgroundColor)
	{
		this.backgroundColor = backgroundColor;
	}
	
	public void setForegroundColor(short foregroundColor)
	{
		this.foregroundColor = foregroundColor;
	}
	
	public void setVisible(boolean visible)
	{
		this.visible = visible;
	}

	public boolean onUp(short x, short y) { return false; }
	public boolean onDown(short x, short y) { return false; }
	public void draw(short x, short y, short w, short h) {}

}
