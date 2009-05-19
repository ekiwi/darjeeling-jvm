package org.csiro.ctp;

import org.csiro.messaging.Packet;

public class CtpPacket extends Packet
{
	
	protected CtpPacket(short size, byte type, short id)
	{
		super(size, type, id);		
	}
	
	protected CtpPacket(short size, byte type, short id, short receiverId)
	{
		super(size, type, id, receiverId);		
	}
	
	protected CtpPacket(byte[] bytes)
	{
		super(bytes);
	}
	
	
	public boolean getRoutingPull()
	{
		return (bytes[Packet.SIZE] & 1)>0;
	}

	public boolean getCongested()
	{
		return (bytes[Packet.SIZE] & 2)>0;
	}
	
	public void setRoutingPull(boolean pull)
	{
		if (pull)
			bytes[Packet.SIZE] &= 1;
		else
			bytes[Packet.SIZE] &= ~1;
	}

	public void setCongested(boolean congested)
	{
		if (congested)
			bytes[Packet.SIZE] &= 2;
		else
			bytes[Packet.SIZE] &= ~2;
	}

}