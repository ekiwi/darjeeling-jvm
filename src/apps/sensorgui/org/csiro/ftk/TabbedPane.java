package org.csiro.ftk;

import javax.oled.Screen;

public class TabbedPane extends Widget
{
	
	private static short BAR_HEIGHT, TEXT_PADDING;
	private byte fontSize = 1;
	private byte selectedIndex, downIndex;

	public TabbedPane(short x, short y, short w, short h)
	{
		super(x, y, w, h);
		selectedIndex = -1;
		// init this here because we don't support static initialisers yet
		// todo: move this to a static initialiser as soon as we have support for it :)
		BAR_HEIGHT = 24;
		TEXT_PADDING = 4;
	}
	
	public TabbedPanePanel addPanel(String text)
	{
		TabbedPanePanel panel = new TabbedPanePanel((short)0, (short)BAR_HEIGHT, w, (short)(h-BAR_HEIGHT), text);
		panel.setShowBorder(true);
		
		if (selectedIndex==-1)
			selectedIndex = 0;
		else
			panel.setVisible(false);
		
		addChild(panel);
		return panel;
	}
	
	public void draw(short x, short y, short w, short h)
	{
		// draw tabstrip
		short px = x;
		for (short i=0; i<children.size(); i++)
		{
			TabbedPanePanel panel = (TabbedPanePanel)children.get(i);
			short sw = (short)(Screen.stringWidth(panel.getText(), fontSize) + TEXT_PADDING*2); 

			Screen.setTextOpaque(false);
			if ((downIndex==i)||(selectedIndex==i))
			{
				Screen.setFill(false);
				Screen.rectangle(px  , y  , px+sw-1, y + BAR_HEIGHT, foregroundColor);
				Screen.rectangle(px+1, y+1, px+sw-2, y + BAR_HEIGHT-1, backgroundColor);
				Screen.setFill(true);
				Screen.rectangle(px+2, y+2, px+sw-3, y + BAR_HEIGHT-2, foregroundColor);
				Screen.putCenteredString(panel.getText(), fontSize, px, y, sw, BAR_HEIGHT, backgroundColor);
			} else {
				Screen.setFill(true);
				Screen.rectangle(px, y, px+sw-1, y + BAR_HEIGHT - 1, backgroundColor);
				Screen.setFill(false);
				Screen.rectangle(px, y, px+sw-1, y + BAR_HEIGHT, foregroundColor);
				Screen.putCenteredString(panel.getText(), fontSize, px, y, sw, BAR_HEIGHT, foregroundColor);
			}
			
			px += sw;
		}
	}
	
	public boolean onDown(short x, short y)
	{
		short oldSelected = selectedIndex;
		downIndex = -1;
		
		if ((y>=0)&&(y<BAR_HEIGHT))
		{
			short px = 0;
			for (short i=0; i<children.size(); i++)
			{
				TabbedPanePanel panel = (TabbedPanePanel)children.get(i);
				short sw = (short)(Screen.stringWidth(panel.getText(), fontSize) + TEXT_PADDING*2); 
				if ((x>=px)&&(x<px+sw)) 
				{
					if (selectedIndex!=-1) ((TabbedPanePanel)children.get(selectedIndex)).setVisible(false);
					selectedIndex = downIndex = (byte)i;
					((TabbedPanePanel)children.get(selectedIndex)).setVisible(true);
				}
				px += sw;
			}
		}
		
		if (selectedIndex!=oldSelected)
			makeDirty();
		
		return true;
	}
	
	public boolean onUp(short x, short y)
	{
		downIndex = -1;
		return true;
	}

}
