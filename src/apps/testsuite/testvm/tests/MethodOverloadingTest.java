package testvm.tests;

import javax.darjeeling.Darjeeling;


public class MethodOverloadingTest
{
	
	public int test()
	{
		return 0;
	}
	 
	public int test(String a)
	{
		return 1;
	}
	 
	public int test(int a, int b)
	{
		return 2;
	}
	 
	public int test(int a, int b, int c)
	{
		return 3;
	}
	 
	
	public static void test(int testBase)
	{
		MethodOverloadingTest test = new MethodOverloadingTest();
		Darjeeling.assertTrue(testBase + 0, test.test()==0);
		Darjeeling.assertTrue(testBase + 1, test.test(null)==1);
		Darjeeling.assertTrue(testBase + 2, test.test(0,0)==2);
		Darjeeling.assertTrue(testBase + 3, test.test(0,0,0)==3);
		
	}

}
