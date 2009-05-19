package javax.fleck.boards ;

// this class provides  wrapper functions around the boards/motorcar.c
// daughterboard driver

public class Motorcar
{
    // calls fos_motorcar_init()
    public static native void initMotorcar();

    /**
     * Returns the value sensed by an infrared sensor. When the way is
     * clear,  returns a small  integer. When  the obstacle  somes very
     * close, returns up to around 1000.
     *
     * The  user should  define  is his  code an  application-specific
     * mapping between directions and the sensor index, eg:
     * <pre>
     *     static final byte LEFT=5;
     *     static final byte FRONT=6;
     * </pre>
     * @param sensor  the index of the sensor on the ADC bay
     * @return the value sensed by the infrared sensor
     * @see FOS documentation (hahaha)
     * */
    public static native short readSensor(byte sensor);

    /**
     * Set the motor to drive forward, backward or to stop
     *
     * @param direction  either DRIVE_FORWARD, DRIVE_BACKWARD, or DRIVE_STOP
     */
    public static native void drive(byte direction);

    /**
     * Set the motor speed. 
     *
     * @param speed Motor speed. Range -255 to 255 (inclusive)
     */
	public static native void setSpeed(short speed);

    /**
     * Steer the front wheels
     *
     * @param direction either STEER_LEFT, STEER_STRAIGHT, or STEER_RIGHT
     */
     public static native void steer(byte direction);

    /**
     * Starts  the PWM,  takes the  brake off,  engages the  motor and
     * powers on the daughterboard.
     */
	public static native void start();

    /**
     * Stops  the PWM,  applies the  brake, disengages  the  motor and
     * powers down the daughterboard.
     */
    public static native void stop();


    
    // remember  that  we have  no  <clinit>,  so  we must  initialize
    // constants explicitely at runtime
    public static byte DRIVE_FORWARD;
	public static byte DRIVE_STOP;
	public static byte DRIVE_BACKWARD;

	public static byte STEER_LEFT;
	public static byte STEER_STRAIGHT;
	public static byte STEER_RIGHT;
    
    public static void init()
    {
        initMotorcar();
        DRIVE_FORWARD=1;
        DRIVE_STOP=0;
        DRIVE_BACKWARD=-1;

        STEER_LEFT=-1;
        STEER_STRAIGHT=0;
        STEER_RIGHT=1;

        
    }
} 

