package javax.fleck ;

/**
 * Controls the leds on the Fleck.
 * 
 * @author Niels Brouwers
 *
 */
public class Leds
{
    public static final byte YELLOW=0;
    public static final byte BLUE=0;
    public static final byte GREEN=1;
    public static final byte RED=2;
    
	/**
	 * Controls a led on the fleck board.
	 * @param led the led to set (0, 1, 2)
	 * @param state the state of the led. (true=on, false=off)
	 */
    public static native void setLed(int led, boolean state);

}
