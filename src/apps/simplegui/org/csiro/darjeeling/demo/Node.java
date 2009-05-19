package org.csiro.darjeeling.demo;


public class Node
{
	
	public short id;
	public short temperature;
	public short memory;
	public int lastHeard;
	public byte[] buttons;
	
	public Node(short id)
	{
		this.id = id;
		buttons = new byte[3];
	}

}
