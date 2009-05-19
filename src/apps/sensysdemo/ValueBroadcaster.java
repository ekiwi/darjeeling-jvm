import javax.fleck.Leds;


public abstract class ValueBroadcaster implements Runnable
{
	
	private int msgtype;
    private int interval;
	private Thread thread;
	
	public ValueBroadcaster()
	{
		this.thread = new Thread(this);
	}
	
	public void start(int msgtype, int interval)
	{
        this.msgtype = msgtype;
        this.interval = interval;
		this.thread.start();
	}

    abstract short[] measure();
    
	public void run()
	{
		while (true)
		{
			
			// Create a broadcast message with sensor data
			SensorMessage message = new SensorMessage(
					(byte)this.msgtype,
                    (short)Darjeeling.getNodeId(),
					this.measure()
					);
			
			// Serialise and send
			message.serialise();
			message.send();
			
			// blink the led to signal the message send event
			//Leds.setLed(0, true);
			Thread.sleep(100);
			//Leds.setLed(0, false);
			
			// sleep the thread for the given interval
			Thread.sleep(interval - 100);
		}		
	}	

}
