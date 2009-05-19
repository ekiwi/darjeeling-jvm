package testvm.tests;

import javax.darjeeling.Darjeeling;

public class InitialiserTest
{
	
	public static int a = 100;
	public static int b;
	
	static
	{
		b = 20;
	}
	
	private static class InitTest
	{
		public int a = 123;
		public static int poepjes = 1;
	}
	
	public static void test(int testBase)
	{
		Darjeeling.assertTrue(testBase + 0, a==100);
		Darjeeling.assertTrue(testBase + 1, b==20);
		Darjeeling.assertTrue(testBase + 2, new InitTest().a==123);
		Darjeeling.assertTrue(testBase + 3, new InitTest().poepjes==1);
		Darjeeling.assertTrue(testBase + 4, InitTest.poepjes==1);
	}

}
