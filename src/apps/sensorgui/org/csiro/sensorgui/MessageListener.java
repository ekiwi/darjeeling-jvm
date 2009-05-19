package org.csiro.sensorgui;

import javax.darjeeling.Darjeeling;
import javax.fleck.Message;
import javax.fleck.Radio;


public class MessageListener implements Runnable
{
	
	private int interval;
	private Thread thread;
	private NodeList nodeList;
	
	public MessageListener(int interval, NodeList nodeList)
	{
		this.interval = interval;
		this.nodeList = nodeList;
		this.thread = new Thread(this);
	}
	
	public void start()
	{
		this.thread.start();
	}

	public void run()
	{
		Darjeeling.print("MAC POLL THREAD\n");
		while (true)
		{
			// check for messages
			Message message = Radio.poll();
			
			if (message!=null) {
	
                SensorMessage sensorMessage = SensorMessage.deSerialise(message);

                short msgtype = sensorMessage.getMsgType();
                Node node = nodeList.getNode(sensorMessage.getNodeId());
                short[] values = sensorMessage.getValues();
                
                Darjeeling.print(String.concat(" msg available, type= ", Integer.toString(msgtype), "\n"));

                if (msgtype == 1) {
                    // Fleck Nano: ax, ay, az
                    node.ax = values[0];
                    node.ay = values[1];
                    node.az = values[2];
                } else if (msgtype == 2) {
                    // Fleck: pot
                    node.pot = values[0];
                } else if (msgtype == 3) {
                    // Fleck: temp, volts, mem
                    node.volts = values[0];
                    node.temperature = values[1];
                    node.memory = values[2];
                }
                
                node.lastHeard = Darjeeling.getTime();
                nodeList.fireChangedEvent();
				
			} else			
				// sleep the selected interval
				Thread.sleep(interval);
		}
	}

}
