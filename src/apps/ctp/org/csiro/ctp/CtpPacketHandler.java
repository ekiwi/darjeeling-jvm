package org.csiro.ctp;

import org.csiro.debug.Debug;
import org.csiro.messaging.Packet;
import org.csiro.messaging.PacketHandler;

public class CtpPacketHandler extends PacketHandler
{
	
	private boolean isCongested;
	private boolean routingPull;
	
	public Packet wrap(byte data[])
	{
		switch (data[0])
		{
			case CtpDataFrame.MESSAGETYPE: return new CtpDataFrame(data);
			case CtpRoutingFrame.MESSAGETYPE: return new CtpRoutingFrame(data);
			case CtpPingFrame.MESSAGETYPE: return new CtpPingFrame(data);
			default: return null;
		}
	}
	
	public void setCongested(boolean isCongested)
	{
		this.isCongested = isCongested;
	}
	
	public void setRoutingPull(boolean routingPull)
	{
		this.routingPull = routingPull;
	}
	
	public boolean isCongested()
	{
		return isCongested;
	}
	
	public boolean isRoutingPull()
	{
		return routingPull;
	}
	
	public void send(CtpPacket packet)
	{
		packet.setCongested(isCongested);
		packet.setRoutingPull(routingPull);
		
		isCongested = false;
		routingPull = false;
		
		super.send(packet);
	}

}
