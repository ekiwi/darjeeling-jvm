package org.csiro.darjeeling.demo;
import javax.fleck.Leds;
import javax.oled.Screen;

public class NodeListPainter implements NodeListListener
{
	
	public void buttonStatusChanged(Node node, byte button)
	{
		Leds.setLed(button, node.buttons[button]!=0);
	}
	
	public void listChanged(NodeList nodeList)
	{
		Screen.clear();
		Screen.setTextOpaque(true);
		
		// paint the node list on the OLED screen
		short x = 4;
		short y = 8;
		
		short col1 = x;
		short col2 = (short)(x + 80);
		short col3 = (short)(x + 160);

		Screen.putLeftCenteredString("Id", 3, col1, y, 1, 1, 0xffff);
		Screen.putLeftCenteredString("Temp", 3, col2, y, 1, 1, 0xffff);
		Screen.putLeftCenteredString("Mem", 3, col3, y, 1, 1, 0xffff);
		y+=24;

		for (Node node : nodeList)
		{
			Screen.putLeftCenteredString(Integer.toString(node.id), 3, col1, y, 1, 1, 0xffff);
			Screen.putLeftCenteredString(Integer.toString(node.temperature), 3, col2, y, 1, 1, 0xffff);
			Screen.putLeftCenteredString(Integer.toString(node.memory), 3, col3, y, 1, 1, 0xffff);
			y+=24;
		}
	}

}
