package org.csiro.darjeeling.demo;


public interface NodeListListener
{
	
	public void listChanged(NodeList nodeList);
	public void buttonStatusChanged(Node node, byte button);

}
