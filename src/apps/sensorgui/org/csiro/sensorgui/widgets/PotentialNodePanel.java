package org.csiro.sensorgui.widgets;

import org.csiro.ftk.Window;
import org.csiro.sensorgui.Node;
import org.csiro.sensorgui.NodeList;

public class PotentialNodePanel extends NodesPanel
{

	public PotentialNodePanel(short x, short y, short w, short h, Window window, NodeList nodeList)
	{
		super(x, y, w, h, window, nodeList);
	}

	public String getHeaderString()
	{
		return "ID     Potential";
	}

	public String getNodeString(Node node)
	{
		return String.concat(
				Integer.toString(node.id),
				"   ",
                Integer.toString(node.pot),
				"   ",
				Integer.toString(node.lastHeard)
				);
	}

}
