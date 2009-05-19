package java.lang;

public class System 
{
	
	// no instance allowed
	private System() {};
	
    public static native int currentTimeMillis();
    
    public static native void arraycopy(Object src, int src_position, Object dst, int dst_position, int length);

    public static void gc()
    {
    	Runtime.getRuntime().gc();
    }


}
