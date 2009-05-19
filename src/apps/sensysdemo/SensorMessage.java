import javax.fleck.BroadcastMessage;
import javax.fleck.Message;

// class to send messages which comprise N shorts
// message on the air has a data payload that comprises:
//    node id: 2 bytes
//    N:       1 byte
//    data:    Nx2 bytes
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
		short dataLength = (short)((values.length<<1) + 2); 
		
		// check precondition, data size cannot exceed maximum payload length
		if (dataLength>javax.fleck.Radio.getMessageLength())
			throw new IllegalArgumentException();
		
		this.msgtype = msgtype;
        this.nodeId = nodeId;
		this.values = values;
        //Darjeeling.print( String.concat(" msg length is ", Integer.toString(dataLength), "\n") );
		
	}

	public void serialise()
	{
		// serialise the data
		allocData((values.length<<1) + 2);
		
		// write nodeId
		writeByte((byte)nodeId);
        
        writeByte(msgtype);
		
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
		return new SensorMessage((byte)0, nodeId, values);
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
