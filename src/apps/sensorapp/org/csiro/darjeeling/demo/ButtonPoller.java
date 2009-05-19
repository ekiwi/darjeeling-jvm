package org.csiro.darjeeling.demo;

import javax.darjeeling.Darjeeling;
import javax.fleck.Leds;
import javax.fleck.TestBoard;

import org.csiro.darjeeling.demo.messagetypes.ControlMessage;

public class ButtonPoller implements Runnable
{
	
	private int interval;
	private Thread thread;
	
	public ButtonPoller(int interval)
	{
		TestBoard.init();
		this.interval = interval;
		this.thread = new Thread(this);
	}
	
	public void start()
	{
		this.thread.start();
	}
	
	public void run()
	{
		byte oldState[] = new byte[3];
		while (true)
		{
			for (byte i=0; i<3; i++)
			{
				byte state = TestBoard.getButtonState(i);
				Leds.setLed(i, state!=0);
				if (state!=oldState[i])
				{
					ControlMessage controlMessage = 
						new ControlMessage((short)Darjeeling.getNodeId(), i, state);
					controlMessage.serialise();
					controlMessage.send();
				}
				oldState[i] = state;
			}
			Thread.sleep(interval);
		}
	}

}
