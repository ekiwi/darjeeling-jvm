package org.csiro.sensorgui.widgets;

import javax.darjeeling.Darjeeling;
import javax.oled.Screen;

import org.csiro.ftk.Widget;

public class GuiPanel extends Widget implements Runnable
{

	public GuiPanel(short x, short y, short w, short h)
	{
		super(x, y, w, h);
		Thread makeDirtyThread = new Thread(this, "status.update");
		makeDirtyThread.start();
	}
	
	public void draw(short x, short y, short w, short h)
	{
		super.draw(x, y, w, h);
		Screen.setTextOpaque(true);
		short lineHeight = 18;
		
		x += 4;
		y += 4;
		Screen.putString(String.concat(
				"Memory: ",
				Integer.toString(Darjeeling.getMemFree()),
				"   "
				), 2, (short)(x), (short)(y), 1, 1, 0xffff);
		y += lineHeight;

		Screen.putString(String.concat(
				"Temperature: ",
				Integer.toString(Darjeeling.getTemperature()),
				"   "
				), 2, (short)(x), (short)(y), 1, 1, 0xffff);
		y += lineHeight;

		Screen.putString(String.concat(
				"Voltage: ",
				Integer.toString(Darjeeling.getVoltage()),
				"   "
				), 2, (short)(x), (short)(y), 1, 1, 0xffff);
		y += lineHeight;

		short nrThreads = Darjeeling.getNrThreads();

		Screen.putString(String.concat(
				"Num threads: ",
				Integer.toString(nrThreads),
				"   "
				), 2, (short)(x), (short)(y), 1, 1, 0xffff);
		y += lineHeight;
		
		for (short i=0; i<nrThreads; i++)
		{
			Thread thread = Darjeeling.getThread(i);
			Screen.putString(Integer.toString(thread.getId()), 1, (short)(x), (short)(y), 1, 1, 0xffff);
			String name = thread.getName();
			String str = String.concat(
					Integer.toString(thread.getId()),
					": ",
					name==null?"<anon>":name
					);
			Screen.putString(str, 1, x, y, 1, 1, 0xffff);
			y += lineHeight;
		}
		
	}

	public void run()
	{
		while (true)
		{
			Thread.sleep(1000);
			this.makeDirty();
		}
	}

}
