/*
 *	Radio.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
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
