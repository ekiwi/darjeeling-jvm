/*
 *	RadioTest.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
import javax.darjeeling.Darjeeling;
import javax.radio.Radio;

import org.csiro.ctp.Ctp;
import org.csiro.ctp.CtpDataConsumer;
import org.csiro.ctp.CtpDataProvider;
import org.csiro.debug.Debug;

public class CtpReimplemented
{

	private static Ctp ctp;
	private static short seq = 0;
	
	private static class ExampleConsumer implements CtpDataConsumer
	{
		public void dataReceived(short[] data, short origin, short timeHasLived)
		{
			Darjeeling.print(String.concat(
					"DATA RECEIVED FROM ID: ",
					Integer.toString(origin),
					" THL: ",
					Integer.toString(timeHasLived),
					" DATA: "
					));
			
				for (int i = 0; i < data.length; i ++)
					Darjeeling.print(String.concat(Integer.toString(data[i]), "."));
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
    	Debug.setDisplayOutput(false);
    	Debug.print(String.concat("Main thread is : ", Integer.toString(Thread.getCurrentThreadId())));
    	Thread.sleep(1000);
    	
    	// Debug.setDisplayOutput(Darjeeling.getNodeId()==1);

    	// create a new CTP object
    	//TODO: please check if getNodeId is implemented in your desired port,
    	//if it is not implemented it returns BASE_CDEF_java_lang_VirtualMachineError
    	if (Darjeeling.getNodeId()==1)
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
