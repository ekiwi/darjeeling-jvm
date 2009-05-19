package org.csiro.darjeeling.demo;

import javax.darjeeling.Darjeeling;
import javax.util.ArrayBag;


public class NodeList extends ArrayBag<Node> implements Runnable
{
	
	private Thread purgeThread;
	private static int TIMEOUT_TIME;
	private ArrayBag<NodeListListener> listeners;
	
	public NodeList()
	{
		TIMEOUT_TIME = 10000;
		this.listeners = new ArrayBag<NodeListListener>();
		this.purgeThread = new Thread(this);
		this.purgeThread.start();
	}
	
	public void addListener(NodeListListener listener)
	{
		this.listeners.add(listener);
	}
	
	public void fireChangedEvent()
	{
		for (short i=0; i<listeners.size(); i++)
			listeners.get(i).listChanged(this);
	}
	
	public void fireButtonStatusChangedEvent(Node node, byte button)
	{
		for (short i=0; i<listeners.size(); i++)
			listeners.get(i).buttonStatusChanged(node, button);
	}
	
	public Node getNode(short id)
	{
		Node node;
		
		// search the node list
		for (short i=0; i<this.size(); i++)
			if ((node=this.get(i)).id==id)
				return node;
		
		// node not found, create one
		node = new Node(id);
		this.add(node);
		return node;	
	}

	public void run()
	{
		while (true)
		{
			boolean removed = false;
			int time = Darjeeling.getTime();
			for (short i=0; i<this.size(); i++)
				if (time-this.get(i).lastHeard>TIMEOUT_TIME)
				{
					this.remove(i);
					i--;
					removed = true;
				}
			if (removed) fireChangedEvent();
			Thread.sleep(5000);
		}
	}

}
