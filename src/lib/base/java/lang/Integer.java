package java.lang;

public class Integer
{
	
	private int value;
	
	public Integer(int value)
	{
		this.value = value;
	}
	
	public int intValue()
	{
		return value;
	}
	
	public static native String toString(int i);

}
