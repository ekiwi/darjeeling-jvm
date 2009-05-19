package javax.fleck ;

/**
 * A message that is broadcasted to all nodes. The destination address defaults 0xFFFF
 * 
 * @author Niels Brouwers
 *
 */
public class BroadcastMessage extends Message
{

	/**
	 * Constructs a new Broadcast message with no payload data.
	 * @param type message type
	 * @param group message group
	 */
	public BroadcastMessage(byte type, byte group)
	{
		super((short)0xffff, type, group);
	}
	
	/**
	 * Constructs a new Broadcast message with no payload data.
	 * @param type message type
	 * @param group message group
	 * @param data the payload data
	 */
	public BroadcastMessage(byte type, byte group, byte[] data)
	{
		super((short)0xffff, type, group, data);
	}
	
}
