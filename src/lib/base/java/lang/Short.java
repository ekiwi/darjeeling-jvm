package java.lang;

public class Short
{
	
	private short value;
	
	public Short(short value)
	{
		this.value = value;
	}
	
	public static Short valueOf(short s)
	{
		return new Short(s);
	}
	
	public short shortValue()
	{
		return value;
	}
	
}
