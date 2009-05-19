import javax.fleck.Radio;
import javax.fleck.Leds;

class ReportPot extends ValueBroadcaster
{
    short[] measure()
    {
        short[] values = new short[1];
        
        values[0] = (short)Darjeeling.getADC( (short)5 );
        
        return values;
    }
}

class ReportStatus extends ValueBroadcaster
{
    short[] measure()
    {
        short[] values = new short[3];
        values[0] = (short)Darjeeling.getBatteryVoltage();
        values[1] = (short)Darjeeling.getTemperature();
        values[2] = (short)Darjeeling.getMemFree();
        
        return values;
    }
}

class Blinker implements Runnable
{
    private int led;
    private int interval;
    private Thread thread;
    
    public Blinker(int led, int interval)
    {
        this.led = led;
        this.interval = interval;
        this.thread = new Thread(this, String.join("blinker", Integer.toString(led)));
        this.thread.start();
    }
    
    public void run()
    {
        while (true)
        {
            Leds.setLed(led, true);
            Thread.sleep(interval);
            Leds.setLed(led, false);
            Thread.sleep(interval);
        }
    }
}

// Demo for Sensys 2008.
//
// Create 5 threads:
//      sample temperature
//      sample battery voltage
//      blink red led
//      blink green led
//      blink blue led
public class SensysDemoApp
{
	public static void main(String[] args)
	{
		
		// friendly welcome message
		Darjeeling.print("Sensys demo app is go ^___^\n");
		
		// set radio channel
		Radio.setChannel((short)0);
		
		// Broadcast values every 5 seconds
		ReportPot pot = new ReportPot();
		pot.start(2, 500);
		
        ReportStatus status = new ReportStatus();
        status.start(3, 5000);
        
        Blinker red =   new Blinker(Leds.RED,   400);
        Blinker green = new Blinker(Leds.GREEN, 500);
        Blinker blue =  new Blinker(Leds.BLUE,  600);
        
        // dont fall off the end of main, should be a Darjeeling primitive
        while (true) {
            Thread.sleep(1000);
        }
	}
}
