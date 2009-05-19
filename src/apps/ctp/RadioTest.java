import javax.darjeeling.Darjeeling;
import javax.radio.Radio;

import org.csiro.ctp.Ctp;
import org.csiro.ctp.CtpDataConsumer;
import org.csiro.ctp.CtpDataProvider;
import org.csiro.debug.Debug;

public class RadioTest
{

	private static Ctp ctp;
	private static short seq = 0;
	
	private static class ExampleConsumer implements CtpDataConsumer
	{
		public void dataReceived(short[] data, short origin, short timeHasLived)
		{
			Debug.print(String.concat(
					"DATA RECEIVED FROM ID: ",
					Integer.toString(origin),
					" THL: ",
					Integer.toString(timeHasLived),
					" DATA: "
					));
			for (byte i=0; i<data.length; i++)
			{
				Debug.print(data[i]);
				Debug.print(" ");
			}
		}
	}

	private static class ExampleProvider implements CtpDataProvider
	{
		public short[] getData()
		{
			Darjeeling.gc();
			return new short[] 
			{
					(short)Darjeeling.getMemFree(),
					seq++,
					ctp.getRoutingEngine().getParent()
			};
		}
	}
	
	public static void main(String args[])
    {
    	Radio.init();

    	// wait for radio to be initialised (hack for TNodes)
    	Thread.sleep(1000);
    	
    	// Debug.setDisplayOutput(Darjeeling.getNodeId()==1);

    	// create a new CTP object
    	if (Darjeeling.getNodeId()==0)
    	{
    		// create sink
        	ctp = new Ctp(new ExampleConsumer());
        	ctp.start();
    	} else
    	{
    		// create leaf
        	ctp = new Ctp(new ExampleProvider());
        	ctp.start();
    	}
    	
    }
	
}

