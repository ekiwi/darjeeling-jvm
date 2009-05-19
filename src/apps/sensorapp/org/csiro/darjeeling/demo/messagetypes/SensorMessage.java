package org.csiro.darjeeling.demo.messagetypes;

import javax.fleck.BroadcastMessage;
import javax.fleck.Message;

public class SensorMessage extends BroadcastMessage
{
	
	public short nodeId;
	public short[] values;

	public SensorMessage(short nodeId, short type, short ... values)
	{
		// type=1, group=0
		super((byte)type, (byte)0x0);
		
		// calculate the message data size
		short dataLength = (short)((values.length<<1) + 2); 
		
		// check precondition, data size cannot exceed maximum payload length
		if (dataLength>javax.fleck.Radio.getMessageLength())
			throw new IllegalArgumentException();
		
		this.nodeId = nodeId;
		this.values = values;
		
	}
	
	public void serialise()
	{
		// serialise the data
		allocData((values.length<<1) + 2);
        
		// write nodeId
		writeByte((byte)nodeId);
		
        // write the message type
        writeByte((byte)this.type);
        
		// write values themselves
		for (short i=0; i<values.length; i++)
			writeShort(values[i]);
	}
	
	public static SensorMessage deSerialise(Message message)
	{
		// start at the beginning of the message
		message.resetDataPointer();
		
		// read sender node Id
		short nodeId = message.readShort();
		
		// read values 
		byte nrValues = message.readByte();
		short[] values = new short[nrValues];
		for (short i=0; i<values.length; i++)
			values[i] = message.readShort();
		
		// construct a new SensorMessage object with these results
		return new SensorMessage((short)nodeId, (short)message.getType(), values);
	}
	
}
