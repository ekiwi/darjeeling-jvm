package testvm.tests;

import javax.fleck.Leds;

public class TryCatchTest
{
	public static void test(int testBase)
	{
		boolean state = true;
		while (true)
		{
			try
			{
				for (short i = 0; i < 3; i++)
				{
					Leds.setLed(i, state);
					Thread.sleep(1000);
				}
				state = !state;
			} catch (Throwable e)
			{
			}
			// state = state;
		}
	}
}
