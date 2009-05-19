package javax.fleck ;

public class Message
{
	
	protected short address;
	protected byte type;
	protected byte group;
	protected byte data[];
	protected short dataPointer;
	
	public Message(short address, byte type, byte group, byte[] data)
	{
		this.address = address;
		this.type = type;
		this.group = group;
		this.data = data;
	}
	
	public Message(short adress, byte type, byte group)
	{
		this(adress, type, group, null);
	}
	
	public void send()
	{
		Radio.sendMessage(address, type, group, data);
	}
	
	public short getAddress()
	{
		return address;
	}
	
	public byte getGroup()
	{
		return group;
	}
	
	public byte getType()
	{
		return type;
	}

	public byte[] getData()
	{
		return data;
	}
    
    public short getLength()
	{
		return (short)data.length;
	}
	
	public void allocData(int size)
	{
		data = new byte[size];
		dataPointer = 0;
	}
	
	public void resetDataPointer()
	{
		dataPointer = 0;
	}
	
	public void writeByte(byte b)
	{
		data[dataPointer] = b;
		dataPointer++;
	}

	public void writeShort(short s)
	{
		data[dataPointer] = (byte)(s>>8);
		data[dataPointer+1] = (byte)(s&255);
		dataPointer+=2;
	}
	
	public byte readByte()
	{
		dataPointer++;
		return data[dataPointer-1];
	}
	
	public short readShort()
	{
		dataPointer+=2;
		return (short)(((data[dataPointer-2]&255)<<8) + (data[dataPointer-1]&255));
	}
	
}
