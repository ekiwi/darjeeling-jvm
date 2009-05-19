package javax.darjeeling ;

public class Darjeeling
{
	
	public static native void assertTrue(int testNr, boolean value);

	public static native void gc();
	
	public static native void print(String str);
	public static native void print(int i);

	public static native int getTime();
	public static native int getMemFree();
	public static native int getNodeId();
	public static native int getBoardTemperature();
	public static native int getVoltage();
	
	public static native int getSecond();
	public static native int getMinute();
	public static native int getHour();

	public static native int getBatteryVoltage();
	public static native int getSolarVoltage();
	public static native int getSolarCurrent();
	public static native int getADC(short chan);

	public static native int random();
	
	public static native short getNrThreads();
	public static native Thread getThread(short nr);
    
    public static native short getEcho(short chan);
    public static native short getHumidity();
    public static native short getTemperature();
    public static native int getPulseCounter();

    public static native void setExpansionPower(short on);
}
