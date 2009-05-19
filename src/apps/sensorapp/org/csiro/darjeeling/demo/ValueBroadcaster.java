package org.csiro.darjeeling.demo;
import javax.darjeeling.Darjeeling;
import javax.fleck.Leds;

import org.csiro.darjeeling.demo.messagetypes.SensorMessage;

public class ValueBroadcaster implements Runnable
{
	
	private int interval;
	private Thread thread;
	
	public ValueBroadcaster(int interval)
	{
		this.interval = interval;
		this.thread = new Thread(this);
	}
	
	public void start()
	{
		this.thread.start();
	}

	public void run()
	{
		while (true)
		{
			// Create a broadcast message with sensor data
			SensorMessage message = new SensorMessage(
					(short)Darjeeling.getNodeId(),
                    (short)0x77, 
					(short)Darjeeling.getTemperature(),
                    (short)Darjeeling.getADC((short)5),
					(short)Darjeeling.getMemFree()
					);			
			// Serialise and send
			message.serialise();
			message.send();
			
			// blink the led to signal the message send event
			Leds.setLed(0, true);
			Thread.sleep(100);
			Leds.setLed(0, false);
			
			// sleep the thread for the given interval
			Thread.sleep(interval - 100);
		}		
	}	

}
