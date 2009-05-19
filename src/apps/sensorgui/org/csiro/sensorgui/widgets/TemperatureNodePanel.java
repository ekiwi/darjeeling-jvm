package org.csiro.sensorgui.widgets;

import org.csiro.ftk.Window;
import org.csiro.sensorgui.Node;
import org.csiro.sensorgui.NodeList;

public class TemperatureNodePanel extends NodesPanel
{

	public TemperatureNodePanel(short x, short y, short w, short h, Window window, NodeList nodeList)
	{
		super(x, y, w, h, window, nodeList);
	}

	public String getHeaderString()
	{
		return("Id    Temp  Mem");
	}

	public String getNodeString(Node node)
	{
		return(String.concat(
				Integer.toString(node.id),
				"   ",
				Integer.toString(node.temperature),
				"    ",
				Integer.toString(node.memory),
				"   "
				));
	}

}
