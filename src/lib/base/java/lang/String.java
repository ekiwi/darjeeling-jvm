package java.lang;

public class String
{

	public native int length();
	public static native String concat(String ... strings);
	public static native String join(String join, String ... strings);
	
	public native boolean equals(String str); 
	
}
