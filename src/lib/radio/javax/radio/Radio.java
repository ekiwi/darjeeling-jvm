package javax.radio;


public class Radio
{
	
	private static native void _waitForMessage();
	private static native byte[] _readBytes();
	private static native void _init();
	private static native byte _getNumMessages();
	public static native void setChannel(short channel);
	public static native short getMaxMessageLength();

	private static Object sendLock, receiveLock;
	
	private Radio()
	{
		// don't let people instantiate this class
	}
	
	public static void init()
	{
		_init();
		sendLock = new Object();
		receiveLock = new Object();
	}
	
	public static Object getSendLock()
	{
		return sendLock;
	}
	
	public static byte getNumMessages()
	{
		return _getNumMessages();
	}
	
	public static byte[] receive()
	{
		if (receiveLock==null)
			throw new RadioNotInitialisedException();
		
		// allow only one thread to wait for radio messages
		synchronized(receiveLock)
		{
			_waitForMessage();
			return _readBytes();
		}		
	}
	
	private static native void _broadcast(byte[] data);
	
	public static void broadcast(byte[] data)
	{
		if (sendLock==null)
			throw new RadioNotInitialisedException();
		
		if (data==null)
			throw new NullPointerException();

		// allow only one thread to send at the same time
		synchronized (sendLock)
		{
			// broadcast the message
			_broadcast(data);
		}
	}
		
	private static native boolean _send(short receiverId, byte[] data);

	public static boolean send(short receiverId, byte[] data)
	{
		if (sendLock==null)
			throw new RadioNotInitialisedException();

		if (data==null)
			throw new NullPointerException();
		
		// allow only one thread to send at the same time
		synchronized (sendLock)
		{
			// broadcast the message
			return _send(receiverId, data);
		}
	}
		
	
}
