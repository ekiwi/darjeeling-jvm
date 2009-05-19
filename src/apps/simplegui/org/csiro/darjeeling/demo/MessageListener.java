package org.csiro.darjeeling.demo;

import javax.darjeeling.Darjeeling;
import javax.fleck.Message;
import javax.fleck.Radio;

import org.csiro.darjeeling.demo.messagetypes.ControlMessage;
import org.csiro.darjeeling.demo.messagetypes.SensorMessage;

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
			
			if (message!=null)
			{
				// Message type 0x1 is SensorMessage
				if (message.getType()==0x1)
				{
					SensorMessage sensorMessage = SensorMessage.deSerialise(message);
					Node node = nodeList.getNode(sensorMessage.nodeId);
					node.lastHeard = Darjeeling.getTime();
					short values[] = sensorMessage.values; 
					node.temperature = values[0];
					node.memory = values[1];
					nodeList.fireChangedEvent();
				}
				// Message type 0x2 is ControlMessage
				if (message.getType()==0x2)
				{
					ControlMessage controlMessage = ControlMessage.deSerialise(message);
					Node node = nodeList.getNode(controlMessage.nodeId);
					node.buttons[controlMessage.button] = controlMessage.state;
					nodeList.fireButtonStatusChangedEvent(node, controlMessage.button);
				}
				
			} else			
				// sleep the selected interval
				Thread.sleep(interval);
		}
	}

}
