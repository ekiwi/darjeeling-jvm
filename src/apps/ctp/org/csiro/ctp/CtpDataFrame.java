package org.csiro.ctp;

import javax.darjeeling.Darjeeling;

import org.csiro.messaging.Packet;


public class CtpDataFrame extends CtpPacket
{
	
	public final static byte MESSAGETYPE = 65;
	
	public CtpDataFrame(short origin, short etx, short seq, short data[])
	{
		super((short)(data.length*2+Packet.SIZE+9), MESSAGETYPE, (short)Darjeeling.getNodeId(), (short)0);

		setOrigin(origin);
		setEtx(etx);
		setSequenceNumber(seq);

		// set data
		setByte((short)(Packet.SIZE+8), (byte)data.length);
		for (short i=0; i<data.length; i++)
			setShort((short)(Packet.SIZE+9+i*2), data[i]);
	}
	
	public CtpDataFrame(byte[] bytes)
	{
		super(bytes);
	}
	
	public void setTimeHasLived(short thl)
	{
		setByte((short)(Packet.SIZE+1), (byte)thl);
	}
	
	public void setEtx(short etx)
	{
		setShort((short)(Packet.SIZE+2), etx);
	}

	public void setOrigin(short origin)
	{
		setShort((short)(Packet.SIZE+4), origin);
	}

	public void setSequenceNumber(short seq)
	{
		setByte((short)(Packet.SIZE+6), (byte)seq);
	}

	public void setCollectId(short cid)
	{
		setByte((short)(Packet.SIZE+7), (byte)cid);
	}
	
	public short getTimeHasLived()
	{
		return getUnsignedByte((short)(Packet.SIZE+1));
	}

	public short getEtx()
	{
		return getShort((short)(Packet.SIZE+2));
	}
	
	public short getOrigin()
	{
		return getShort((short)(Packet.SIZE+4));
	}
	
	public short getSequenceNumber()
	{
		return getUnsignedByte((short)(Packet.SIZE+6));
	}
	
	public short getCollectionId()
	{
		return getUnsignedByte((short)(Packet.SIZE+7));
	}
	
	public short getDataLength()
	{
		return getUnsignedByte((short)(Packet.SIZE+8));
	}

	public short getDataItem(byte i)
	{
		return getShort((short)(Packet.SIZE + 9 + i*2));
	}
	
	

}