package org.csiro.ftk;
import javax.oled.Screen;



public class Label extends Widget
{

	private String text;
	private byte font;
	private boolean opaque = false;
	private short padding;
	
	public Label(short x, short y, byte font, String text)
	{
		this(x,y,Screen.stringWidth(text, font),Screen.stringHeight(font), font, text);
	}
	
	public Label(short x, short y, short w, short h, byte font, String text)
	{
		super(x,y,w,h);
		this.font = font;
		this.text = text;
	}
	
	public void setText(String text)
	{
		this.text = text;
		this.dirty = true;
	}
	
	public void draw(short x, short y, short w, short h)
	{
		if (opaque) 
		{
			Screen.setFill(true);
			Screen.rectangle(x, y, x+w-1, y+h-1, backgroundColor);
		}
		Screen.putLeftCenteredString(text, font, x+padding, y+padding, w-padding*2, h-padding*2, foregroundColor);		
	}
	
	public void setOpaque(boolean opaque)
	{
		this.opaque = opaque;
	}
	
	public void setFont(short font)
	{
		this.font = (byte)font;
	}
	
	public void setPadding(short padding)
	{
		this.padding = padding;
	}
	
}
