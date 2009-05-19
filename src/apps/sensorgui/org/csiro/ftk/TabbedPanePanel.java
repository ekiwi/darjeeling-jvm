package org.csiro.ftk;

public class TabbedPanePanel extends Panel
{
	
	private String text;
	
	public TabbedPanePanel(short x, short y, short w, short h, String text)
	{
		super(x,y,w,h);		
		this.text = text;
	}
	
	public String getText()
	{
		return text;
	}

}
