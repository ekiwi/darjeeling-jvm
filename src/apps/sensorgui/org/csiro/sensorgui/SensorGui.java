package org.csiro.sensorgui;
import javax.fleck.Radio;
import javax.oled.Screen;
import javax.oled.TouchScreen;

import org.csiro.ftk.TabbedPane;
import org.csiro.ftk.TabbedPanePanel;
import org.csiro.ftk.Window;
import org.csiro.sensorgui.widgets.AccelerationNodePanel;
import org.csiro.sensorgui.widgets.GuiPanel;
import org.csiro.sensorgui.widgets.PotentialNodePanel;
import org.csiro.sensorgui.widgets.StatusNodePanel;
import org.csiro.sensorgui.widgets.TemperatureNodePanel;

public class SensorGui
{
	
	private Window mainWindow;
	
	private SensorGui()
	{
		NodeList nodeList = new NodeList();
		
		// start the message listener
		MessageListener messageListener = new MessageListener(20, nodeList);
		messageListener.start();
		
		mainWindow = new Window((short)0, (short)0, (short)240, (short)320); 
		// mainWindow.setBackgroundColor((short)0x7f7f);
		
		TabbedPane tabs = new TabbedPane((short)0, (short)20, (short)240, (short)300);

		// node page
		TabbedPanePanel nodesPane = tabs.addPanel("Pot");
		nodesPane.addChild(new PotentialNodePanel((short)2, (short)2, (short)236, (short)226, mainWindow, nodeList));

		nodesPane = tabs.addPanel("Temp");
		nodesPane.addChild(new TemperatureNodePanel((short)2, (short)2, (short)236, (short)226, mainWindow, nodeList));
		
		nodesPane = tabs.addPanel("Status");
		nodesPane.addChild(new StatusNodePanel((short)2, (short)2, (short)236, (short)226, mainWindow, nodeList));
		
		nodesPane = tabs.addPanel("Acc");
		nodesPane.addChild(new AccelerationNodePanel((short)2, (short)2, (short)236, (short)226, mainWindow, nodeList));
		
		// status page
		TabbedPanePanel statusPane = tabs.addPanel("Gui");
		GuiPanel statusPanel = new GuiPanel((short)2, (short)2, (short)236, (short)226);
		statusPane.addChild(statusPanel);

		mainWindow.addChild(tabs);
		mainWindow.update();
		
		// create a thread that automatically updates the screen every 1 seconds
		Thread updateThread = new Thread(new Runnable() {
			public void run()
			{
				while (true)
				{
					mainWindow.update();
					Thread.sleep(2500);
				}
			}
		});
		updateThread.start();
		
	}
	
	public static void main(String args[])
	{
		// init radio channel
		Radio.setChannel((short)1);

		// init the OLED screen
		Screen.init();
		Screen.clear();
		
		// init the touch screen
		TouchScreen.init();
		
		// create the GUI 
		new SensorGui();
		
		// start the touch screen polling loop
		TouchScreen.start();
		
	}

}
