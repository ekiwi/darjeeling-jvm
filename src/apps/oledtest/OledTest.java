
import javax.fleck.* ;
import javax.oled.* ;
import java.lang.*;

public class OledTest
{
	
	public static class TouchScreenHandler implements TouchScreenListener 
	{
		public void touchScreenDown(short x, short y)
		{
	    	Screen.circle(x,y,10,31<<11);
		}
		
		public void touchScreenUp(short x, short y)
		{
	    	//Screen.rectangle(x-10,y-10,x+10,y+10,31);
		}
	}
	
    public static void main(String args[])
    {
        Leds.setLed(Leds.RED,true);
        
    	// first call init() to initialise the screen
    	Screen.init();

        Leds.setLed(Leds.YELLOW,true);

        Screen.clear();
    	
    	// init the touch screen
    	TouchScreen.init();
    	TouchScreen.addListener(new TouchScreenHandler());
    	
    	// start the polling thread
    	TouchScreen.start();
    	
    	// draw some primitives
    	Screen.putString("This is the oledtest application",0,0,0,1,1,0xffff);
        Screen.rectangle(20,60,220,260,31);
    	Screen.circle(120,160,100,63<<5);

        Leds.setLed(Leds.GREEN,true);

        
    	// wait for touchscreen events
        int x=0;
    	while (true)
    	{
            Thread.sleep(1000);
            Screen.rectangle(x,270,x+1,275,0);
            x++;
            Screen.rectangle(x,270,x+1,275,31<<11);
            if(x>100)
                x=50;

            Leds.setLed(x%3,x%2==0);

    	}
    }
}
