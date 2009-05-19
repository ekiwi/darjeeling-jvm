package javax.fleck ;

public interface RadioMessageListener
{
	
	public void receive(short type, short from, short group, byte[] message);

}
