package testvm.tests;

import javax.darjeeling.Darjeeling;

import testvm.classes.Child;


public class BitManipulationTest {
	public static void test(int testBase)
	{
		Child child = new Child(new byte[4]);
		for (int i = 0; i < 10; i ++){
			child.getBytes()[3] = 6;
			child.setCongested(true);
			Darjeeling.assertTrue(testBase, child.getCongested());
			child.setCongested(false);
			Darjeeling.assertTrue(testBase + 1, !child.getCongested());
//			child.setRoutingPull(true);
/*			Darjeeling.assertTrue(testBase + 2, child.getRoutingPull());
			child.setRoutingPull(false);
			Darjeeling.assertTrue(testBase + 3, !child.getRoutingPull());*/
		}
	}
}
