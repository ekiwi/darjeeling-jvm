package testvm.classes;



public abstract class Parent
{
	
	protected byte[] bytes;
	public final static short SIZE = 3;
	public final static short BROADCAST_ID = -1;
	private short receiverId;
	
	protected Parent(short size, byte id, short senderId, short receiverId)
	{
		bytes = new byte[size];
		bytes[0] = id;
		setShort((short)1, senderId);
		this.receiverId = receiverId;
	}
	
	protected Parent(short size, byte id, short senderId)
	{
		bytes = new byte[size];
		bytes[0] = id;
		setShort((short)1, senderId);
		receiverId = BROADCAST_ID;
	}
	
	protected Parent(byte[] bytes)
	{
		this.bytes = bytes;
	}
	
	public short getReceiverId()
	{
		return receiverId;
	}
	
	public void setReceiverId(short receiverId)
	{
		this.receiverId = receiverId;
	}
	
	public boolean isBroadcast()
	{
		return receiverId == BROADCAST_ID;
	}
	
	public byte getPacketId()
	{
		return bytes[0];
	}
	
	public short getSenderId()
	{
		return getShort((short)1);
	}
	
	public void setSenderId(short id)
	{
		setShort((short)1, id);
	}
	
	public byte[] getBytes()
	{
		return bytes;
	}
	
	protected byte getByte(short offset)
	{
		return bytes[offset];
	}

	protected short getUnsignedByte(short offset)
	{
		return (short)(bytes[offset] & 255);
	}
	
	protected short getShort(short offset)
	{
		return (short)((bytes[offset+1]&255) + ((bytes[offset]&255)<<8)); 
	}

	protected int getUnsignedShort(short offset)
	{
		return (int)((bytes[offset+1]&255) + ((bytes[offset]&255)<<8)); 
	}

	protected void setByte(short offset, byte value)
	{
		bytes[offset] = value;
	}

	protected void setShort(short offset, short value)
	{
		bytes[offset] = (byte)(value >> 8); 
		bytes[offset+1] = (byte)(value); 
	}
	
}