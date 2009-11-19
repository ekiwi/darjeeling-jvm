package testvm.tests;

import javax.darjeeling.Darjeeling;

import testvm.classes.Child;


public class BitManipulationTest {
	public static void test(int testBase)
	{
		Child child = new Child(new byte[4]);
		for (int i = 0; i < 10; i ++){
			for (int j = 0; j < child.getBytes().length; j ++)
			child.setCongested(true);
			child.getBytes()[0] = 0;
			Darjeeling.assertTrue(testBase + i*4, child.getCongested());
			child.setCongested(false);
			Darjeeling.assertTrue(testBase + i*4 + 1, !child.getCongested());
			child.setRoutingPull(true);
			Darjeeling.assertTrue(testBase + i*4 + 2, child.getRoutingPull());
			child.setRoutingPull(false);
			Darjeeling.assertTrue(testBase + i*4 + 3, !child.getRoutingPull());
		}
	}
}
