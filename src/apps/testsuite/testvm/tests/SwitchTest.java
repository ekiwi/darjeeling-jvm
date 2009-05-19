package testvm.tests;

import javax.darjeeling.Darjeeling;

public class SwitchTest
{

	public static void test(int testBase)
	{
		
		// tableswitch
		int testNr = testBase;
		for (int i=-1; i<4; i++)
		{
			switch(i)
			{
				case 0: Darjeeling.assertTrue(testNr, i==0); break;
				case 1: Darjeeling.assertTrue(testNr, i==1); break;
				case 2: Darjeeling.assertTrue(testNr, i==2); break;
				case 3: Darjeeling.assertTrue(testNr, i==3); break;
				default: Darjeeling.assertTrue(testNr, i<0||i>=8); break;
			}
			testNr++;
		}
		
		testNr = testBase + 10;
		for (int i=-100; i<900; i+=100)
		{
			switch(i)
			{
				case 000: Darjeeling.assertTrue(testNr, i==000); break;
				case 100: Darjeeling.assertTrue(testNr, i==100); break;
				case 200: Darjeeling.assertTrue(testNr, i==200); break;
				case 300: Darjeeling.assertTrue(testNr, i==300); break;
				case 400: Darjeeling.assertTrue(testNr, i==400); break;
				case 500: Darjeeling.assertTrue(testNr, i==500); break;
				case 600: Darjeeling.assertTrue(testNr, i==600); break;
				case 700: Darjeeling.assertTrue(testNr, i==700); break;
				default: Darjeeling.assertTrue(testNr, i<0||i>700); break;
			}
			testNr++;
		}
		
	}

}
