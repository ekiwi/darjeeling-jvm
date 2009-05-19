import javax.fleck.Leds;

public class Blink
{
    public static void main(String args[])
    {
        boolean state=true;
        while(true)
        {
            for (short i=0; i<3; i++)
            {
	        	Leds.setLed(i,state);
	            Thread.sleep(1000);
            }
            state=!state;
        }
    }
}
