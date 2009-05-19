package org.csiro.sensorgui.widgets;

import javax.oled.Screen;

import org.csiro.ftk.Widget;
import org.csiro.ftk.Window;
import org.csiro.sensorgui.Node;
import org.csiro.sensorgui.NodeList;
import org.csiro.sensorgui.NodeListListener;

public abstract class NodesPanel extends Widget implements NodeListListener
{

	private Window window;
	private NodeList nodeList;
	private int previousNodeCount = 0;
	
	public NodesPanel(short x, short y, short w, short h, Window window, NodeList nodeList)
	{
		super(x, y, w, h);
		this.window = window;
		this.nodeList = nodeList;
		nodeList.addListener(this);
	}
	
	public void draw(short x, short y, short w, short h)
	{
		super.draw(x, y, w, h);
		short lineHeight = 18;
		
		Screen.setBackgroundColor(0x0);
		Screen.setTextOpaque(true);

		x += 4;
		y += 4;
		Screen.putString(getHeaderString(), 2, x, y, 1, 1, 0xffff);
		y += lineHeight;

		short nodeCount = nodeList.size();
		for (short i=0; i<nodeCount; i++)
		{
			Node node = nodeList.get(i);
			Screen.putString(getNodeString(node), 2, x, y, 1, 1, 0xffff);
			y+=lineHeight;
		}

		if (previousNodeCount>nodeCount)
		{
			Screen.setFill(true);
			Screen.rectangle(x, y, (short)(x + w - 8), (short)(y + lineHeight * (previousNodeCount-nodeCount)), 0x0);
		}
		
		previousNodeCount = nodeCount;
		
	}
	
	public abstract String getHeaderString();
	public abstract String getNodeString(Node node);

	public void listChanged(NodeList nodeList)
	{
		makeDirty();
	}

}
