package org.csiro.sensorgui;
import javax.fleck.BroadcastMessage;
import javax.fleck.Message;

public class SensorMessage extends BroadcastMessage
{
	
	private short nodeId;
    private byte msgtype;
	private short[] values;

	public SensorMessage(byte msgtype, short nodeId, short ... values)
	{
		// type=1, group=0
		super((byte)0x1, (byte)0x0);
		
		// calculate the message data size
		short dataLength = (short)((values.length<<1) + 3); 
		
		// check precondition, data size cannot exceed maximum payload length
		if (dataLength>javax.fleck.Radio.getMessageLength())
			throw new IllegalArgumentException();
		
		this.msgtype = msgtype;
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
		short nodeId = message.readByte();
        
        // read message type
        byte msgtype = message.readByte();
		
		// read values 
		//byte nrValues = message.readByte();
		byte nrValues = (byte) ((message.getLength() - 2) >>> 1);
		short[] values = new short[nrValues];
		for (short i=0; i<values.length; i++)
			values[i] = message.readShort();
		
		// construct a new SensorMessage object with these results
		return new SensorMessage(msgtype, nodeId, values);
	}
	
	public short getNodeId()
	{
		return nodeId;
	}
    
	public short getMsgType()
	{
		return msgtype;
	}

	public short[] getValues()
	{
		return values;
	}
}
