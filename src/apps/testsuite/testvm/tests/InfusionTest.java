package testvm.tests;

import javax.darjeeling.Darjeeling;
import javax.darjeeling.vm.Infusion;
import javax.darjeeling.vm.InfusionUnloadDependencyException;

public class InfusionTest
{
	
//	public static void printInfusion(String prefix, Infusion infusion)
//	{
//		Darjeeling.print(prefix);
//		Darjeeling.print(infusion.getName());
//		Darjeeling.print("\n");
//		
//		// iterate over imported infusion list
//		for (short j=0; j<infusion.getImportedInfusionCount(); j++)
//		{
//			Infusion importedInfusion = infusion.getImportedInfusion(j);
//			printInfusion(String.concat(prefix, "\t"), importedInfusion);
//		}
//	}
	
	public static void test(int testBase)
	{
		
		Infusion base = Infusion.getInfusionByName("base");
		
		Darjeeling.assertTrue(testBase + 0, base!=null);
		Darjeeling.assertTrue(testBase + 1, (base!=null) && (base.getImportedInfusionCount()==0));
		
		try {
			base.unload();
			Darjeeling.assertTrue(testBase + 2, false);
		} catch (InfusionUnloadDependencyException ex)
		{
			Darjeeling.assertTrue(testBase + 2, true);
		}
		
		/*
		Infusion testSuite = Infusion.getInfusionByName("testsuite");
		try {
			testSuite.unload();
			Darjeeling.assertTrue(testBase + 3, false);
		} catch (InfusionUnloadDependencyException ex)
		{
			Darjeeling.assertTrue(testBase + 3, false);
		}
		*/
		
	}

}
