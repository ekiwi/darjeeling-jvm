/**
 * Implementation of springbrook weather station applications apps/springbrook/trunk/src/weatherstation.c
 * in Java using Darjeeling.
 */
import javax.darjeeling.*;
import javax.fleck.Leds;
import javax.radio.Radio;

import org.csiro.ctp.*;
import org.csiro.debug.*;
import org.csiro.messaging.*;

class DataThread implements Runnable
{
    public void run()  {
        int i=0;
        boolean state=true;
        
        Leds.setLed(Leds.GREEN, true);

        while (true) {
            int count;

            Darjeeling.print("DataThread: Reading sensors\n");
                        
            Darjeeling.setExpansionPower((short)1);
            
            TDF.put( TDF.BATTERY_VOLTAGE, Darjeeling.getBatteryVoltage());   // battery voltage
            TDF.put( TDF.SOLAR_VOLTAGE,   Darjeeling.getSolarVoltage());   // solar voltage
            TDF.put( TDF.SOLAR_CURRENT,   Darjeeling.getSolarCurrent());   // solar current
            
            TDF.put( TDF.MOISTURE_ECHO_1,  Darjeeling.getEcho((short)0));  // soil moisture
            TDF.put( TDF.LEAF_WETNESS, Darjeeling.getEcho((short)1) );     // leaf wetness
            
            TDF.put( TDF.HUMIDITY_TEMP, Darjeeling.getTemperature() );    // temperature
            TDF.put( TDF.HUMIDITY, Darjeeling.getHumidity() );   // humidity
            
            TDF.put( TDF.WIND_DIRECTION, Darjeeling.getEcho((short)2) );   // wind direction

            Darjeeling.getPulseCounter();               // reset the wind speed counter
            Thread.sleep(5000);

            count = Darjeeling.getPulseCounter();       // reset the wind speed counter
            TDF.put( TDF.WIND_SPEED, count*10/5 );   // wind speed

            
            Darjeeling.setExpansionPower((short)0);


            Thread.sleep(1000);

           
            Leds.setLed(Leds.YELLOW, state);

            state = !state;

        }
    }
}




public class Springbrook
{

	private static class SpringbrookDataProvider implements CtpDataProvider
	{
		public short[] getData()
		{
            // This hard-coded '3' hackish. But the fact is that there
            // is  15  bytes  of  payload  space  in  Darjeeling/Fleck
            // messages, which is enough  for 3 (SID, value)-pairs but
            // not for four.
            return TDF.read(3);

		}
	}
	
    private static class SpringbrookDataConsumer implements CtpDataConsumer
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
			for (byte i=0; i<data.length; i+=2)
			{
				Debug.print("(");
                Debug.print(data[i]);
                Debug.print(",");
				Debug.print(data[i+1]);
                Debug.print(") ");
			}
			Debug.print("\n");
		}

    }

    public static void main(String args[])
    { 
    	Radio.init();

        // allocate temporary storage for 9 different TDF pairs
        TDF.init(9);
       
        Leds.setLed(Leds.RED, true);

        Debug.print("Springbrook node ");
        Debug.print(Darjeeling.getNodeId());
        Debug.print(" starting\n");
            

        // create a new CTP object
    	if (Darjeeling.getNodeId()==0)
    	{
    		// create sink
        	Ctp ctp = new Ctp(new SpringbrookDataConsumer());
        	ctp.start();
    	} else
    	{
    		// create leaf
        	Ctp ctp = new Ctp(new SpringbrookDataProvider());
        	ctp.start();

            /* spawn the data gathering thread */
            DataThread  datathread = new DataThread();
            new Thread(datathread).start();
    	}
   

        /* spawn the watchdog thread */
    }
}
