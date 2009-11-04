package testvm.classes;

public class Child extends Parent
{
	
	protected Child(short size, byte type, short id)
	{
		super(size, type, id);		
	}
	
	protected Child(short size, byte type, short id, short receiverId)
	{
		super(size, type, id, receiverId);		
	}
	
	public Child(byte[] bytes)
	{
		super(bytes);
	}
	
	
	public boolean getRoutingPull()
	{
		return (bytes[Parent.SIZE] & 1)>0;
	}

	public boolean getCongested()
	{
		return (bytes[Parent.SIZE] & 2)>0;
	}
	
		public void setRoutingPull(boolean pull)
		{
			if (pull){
				bytes[Parent.SIZE] &= 1;
			}
			else{
				bytes[Parent.SIZE] &= ~1;
			}
		}

	public void setCongested(boolean congested)
	{
		if (congested){
			bytes[Parent.SIZE] &= 2;
		}
		else{
			bytes[Parent.SIZE] &= ~2;
		}
	}

}