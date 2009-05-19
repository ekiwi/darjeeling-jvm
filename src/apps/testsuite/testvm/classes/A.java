package testvm.classes;

public class A implements AInterface
{
	public static int integer1, integer2, integer3; 
	public static byte byte1, byte2, byte3; 
	public static short short1, short2, short3; 
	public static boolean boolean1, boolean2, boolean3; 
	public static int[] inta1, inta2, inta3; 
	
	public int x,y;
	
	public class Inner
	{
		
		public int getX()
		{
			return x;
		}
		
	}
	
	int a;
	
	public A(int x, int y)
	{
		this.x = x;
		this.y = y;
	}
	
	public A()
	{
		this.x = 2; 
		this.y = 3;
	}
	
	public int getX()
	{
		return x;
	}
	
	public int getY()
	{
		return y;
	}
	
	public int getSquaredLength()
	{
		return x*x + y*y;
	}
	
	public int virtualMethod()
	{
		return 0;
	}

	public int AInterfaceMethod()
	{
		return 0;
	}
	
	public Inner createInner()
	{
		return new Inner();
	}


}
