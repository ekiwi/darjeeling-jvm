package java.lang;


public class Object
{
	
	public Object()
	{
		
	}
	
	public native void wait(int timeOut);
	public native void wait();
	public native void notify();
	public native void notifyAll();
	
	public String toString()
	{
		return "Object";
	}

}
