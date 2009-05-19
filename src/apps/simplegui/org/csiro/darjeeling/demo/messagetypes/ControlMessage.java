package org.csiro.darjeeling.demo.messagetypes;
import javax.fleck.BroadcastMessage;
import javax.fleck.Message;


public class ControlMessage extends BroadcastMessage
{

	public byte button, state;
	public short nodeId;
	
	public ControlMessage(short nodeId, byte button, byte state)
	{
		// type=1, group=0
		super((byte)0x2, (byte)0x0);
		this.nodeId = nodeId;
		this.button = button;
		this.state = state;
	}

	public void serialise()
	{
		// serialise the data
		allocData(4);
		
		writeShort(nodeId);
		writeByte(button);
		writeByte(state);
	}
	
	public static ControlMessage deSerialise(Message message)
	{
		// start at the beginning of the message
		message.resetDataPointer();
		
		// read sender node Id
		short nodeId = message.readShort();
		byte button = message.readByte(); 
		byte state = message.readByte(); 
		
		// construct a new SensorMessage object with these results
		return new ControlMessage(nodeId, button, state);
	}
	
}
