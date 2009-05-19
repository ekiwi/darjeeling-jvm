package testvm.tests;

import javax.darjeeling.Darjeeling;

public class ThreadTest
{
	
	private int syncTest;
	private Object obj;
	private int numThreads;
	
	private class TestThread implements Runnable
	{
		public int testbase;
		
		public TestThread(int testbase)
		{
			this.testbase = testbase;
		}
		
		public void run()
		{
			try {
				Object obj2 = new Object();
				try {
					synchronized (obj2)
					{
						synchronized(obj)
						{
							syncTest++;
							Thread.sleep(100);
							Darjeeling.assertTrue(testbase, syncTest==1);
							syncTest--;
						}
					}
				} catch (Error e)
				{
					// Darjeeling.print(e.toString());
				} finally
				{
				}
				
			} catch (Throwable t)
			{
				// do nothing
			} finally
			{
				numThreads--;
			}
		}
		
	}

	public void runTest(int testbase)
	{
		numThreads = 0;
		obj = new Object();
		for (int i=0; i<10; i++)
		{
			TestThread test = new TestThread(testbase + i);
			Thread t = new Thread(test);
			t.start();
			numThreads++;
		}
		
		// wait for all threads to finish (we should have a Thread.join?)
		while (numThreads>0) 
			Thread.sleep(10);
	}
	
	public static void test(int testbase)
	{
		new ThreadTest().runTest(testbase);
	}

}
