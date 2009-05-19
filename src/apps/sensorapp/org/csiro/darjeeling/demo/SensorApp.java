package org.csiro.darjeeling.demo;
import javax.darjeeling.Darjeeling;
import javax.fleck.Radio;

public class SensorApp
{
	
	public static void main(String[] args)
	{
		
		// friendly welcome message
		Darjeeling.print("Sensor app is go ^___^\n");
		
		// set radio channel
		Radio.setChannel((short)1);
		
		// Broadcast values every 5 seconds
		ValueBroadcaster broadCaster = new ValueBroadcaster(5000);
		broadCaster.start();
		
		// Poll the buttons on the test board (if present) and relay changes
		//ButtonPoller buttonPoller = new ButtonPoller(50);
		//buttonPoller.start();
		
	}

}
