package org.csiro.sensorgui;

public class Node
{
	
	public short id;
	public short temperature;
	public short memory;
    public short pot;
    public short volts;
    public short ax, ay, az;
	public int lastHeard;
	
	public Node(short id)
	{
		this.id = id;
	}

}
