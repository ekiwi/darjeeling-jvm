package org.csiro.ctp;

import org.csiro.messaging.Packet;

public class CtpPingFrame extends CtpPacket
{
	
	public final static byte MESSAGETYPE = 66;
	
	public CtpPingFrame(short senderId, short receiverId)
	{
		super((short)(Packet.SIZE + 1), MESSAGETYPE, senderId, receiverId);
	}
	
	public CtpPingFrame(byte bytes[])
	{
		super(bytes);
	}


}