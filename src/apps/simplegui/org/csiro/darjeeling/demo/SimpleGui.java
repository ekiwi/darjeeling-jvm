package org.csiro.darjeeling.demo;
import javax.fleck.Radio;
import javax.oled.Screen;


public class SimpleGui
{
	
    public static void main(String args[])
    {
    	
    	// Initialise the OLED screen
    	Screen.init();
    	
    	// Initialise the MAC layer
    	Radio.setChannel((short)1);
    	
    	// create a NodeList object to hold our Nodes
    	NodeList nodeList = new NodeList();
    	
    	// Create a MessageListener that will poll the MAC layer for
    	// messages and parse them as they come in. The first parameter
    	// sets the polling interval (20 milliseconds) 
    	MessageListener messageListener = new MessageListener(20, nodeList);
    	messageListener.start();
    	
    	// Create a NodeListPainter object that will paint the state of the NodeList
    	// on the OLED screen. It updates automatically.
        NodeListPainter nodeListPainter = new NodeListPainter();
        nodeList.addListener(nodeListPainter);
    	
    }
}
