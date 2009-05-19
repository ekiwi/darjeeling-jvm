package javax.fleck ;

public class Radio
{

	public native static void setChannel(short channel);
	public static native short getMessageLength();
	
	private static native void _sendMessage(short addr, byte type, byte group, byte[] message);
	private static native boolean bufferHasMessages();
	private static native short bufferGetTopAddress();
	private static native byte bufferGetTopGroup();
	private static native byte bufferGetTopType();
	private static native byte[] bufferGetTopBytes();
	private static native void bufferPop();

	public static void sendMessage(short addr, byte type, byte group, byte[] message)
	{
		// TODO check for null, size
		_sendMessage(addr, type, group, message);
	}
	
	public static Message poll()
	{
		Message ret = null;
		if (bufferHasMessages())
		{
            ret = new Message(
					bufferGetTopAddress(),
					bufferGetTopType(),
					bufferGetTopGroup(),
					bufferGetTopBytes()
					);
			bufferPop();
		}
		return ret;
	}

}
