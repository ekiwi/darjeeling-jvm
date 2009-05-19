package org.csiro.ctp;

import javax.darjeeling.Darjeeling;

import org.csiro.messaging.Packet;


public class CtpRoutingFrame extends CtpPacket
{
	
	public final static byte MESSAGETYPE = 64;
	
	public CtpRoutingFrame(short etx)
	{
		super((short)(Packet.SIZE+5), MESSAGETYPE, (short)Darjeeling.getNodeId());
		this.setEtx(etx);
	}
	
	public CtpRoutingFrame(byte[] bytes)
	{
		super(bytes);
	}
	
	public void setParent(short parent)
	{
		setShort((short)(Packet.SIZE+1), parent);
	}
	
	public void setEtx(short etx)
	{
		setShort((short)(Packet.SIZE+3), etx);
	}
	
	public short getParent()
	{
		return getShort((short)(Packet.SIZE+1));
	}

	public short getEtx()
	{
		return getShort((short)(Packet.SIZE+3));
	}

}