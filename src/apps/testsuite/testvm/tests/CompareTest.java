package testvm.tests;

import javax.darjeeling.Darjeeling;

public class CompareTest
{
	
	public static void test(int testBase)
	{
		// test equal
		if (1==1) Darjeeling.assertTrue(testBase + 0, true); else 
			Darjeeling.assertTrue(testBase + 0, false);
		if (1==2) Darjeeling.assertTrue(testBase + 1, false); else 
			Darjeeling.assertTrue(testBase + 1, true);

		// test not equal
		if (1!=2) Darjeeling.assertTrue(testBase + 2, true); else 
			Darjeeling.assertTrue(testBase + 2, false);
		if (1!=1) Darjeeling.assertTrue(testBase + 3, false); else 
			Darjeeling.assertTrue(testBase + 3, true);
		
		// null test
		Object o = null;
		Darjeeling.assertTrue(testBase + 4, o==null);
		o = new Object();
		Darjeeling.assertTrue(testBase + 5, o!=null);

	}	

}
