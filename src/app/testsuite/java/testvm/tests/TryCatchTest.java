package testvm.tests;

import javax.darjeeling.actuators.Leds;

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
					Leds.set(i, state);
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
