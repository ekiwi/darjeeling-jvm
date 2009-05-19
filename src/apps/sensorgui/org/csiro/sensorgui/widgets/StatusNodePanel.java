package org.csiro.sensorgui.widgets;

import org.csiro.ftk.Window;
import org.csiro.sensorgui.Node;
import org.csiro.sensorgui.NodeList;

public class StatusNodePanel extends NodesPanel
{

	public StatusNodePanel(short x, short y, short w, short h, Window window, NodeList nodeList)
	{
		super(x, y, w, h, window, nodeList);
	}

	public String getHeaderString()
	{
		return "ID     ";
	}

	public String getNodeString(Node node)
	{
		return String.concat(
				
				);
	}

}
