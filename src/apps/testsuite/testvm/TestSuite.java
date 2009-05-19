package testvm;

import javax.darjeeling.Darjeeling;

import testvm.classes.TreeNode;
import testvm.tests.ArithmeticTest;
import testvm.tests.ArrayTest;
import testvm.tests.ClassHierarchyTest;
import testvm.tests.CompareTest;
import testvm.tests.ExceptionsTest;
import testvm.tests.FieldTest;
import testvm.tests.GarbageCollectionTest;
import testvm.tests.InfusionTest;
import testvm.tests.InheritanceTest;
import testvm.tests.InitialiserTest;
import testvm.tests.InvokeVirtualTest;
import testvm.tests.MD5Test;
import testvm.tests.MethodOverloadingTest;
import testvm.tests.RuntimeExceptionsTest;
import testvm.tests.StaticFieldsTest;
import testvm.tests.SwitchTest;
import testvm.tests.ThreadTest;

public class TestSuite
{
	
	public static void test()
	{
		ArithmeticTest.test(000);
		ArrayTest.test(100);
		CompareTest.test(200);
		ClassHierarchyTest.test(300);
		StaticFieldsTest.test(400);
		FieldTest.test(500);
		InitialiserTest.test(600);
		InvokeVirtualTest.test(700);
		GarbageCollectionTest.test(800);
		SwitchTest.test(900);
		ThreadTest.test(1000);
		InheritanceTest.test(1100);
		ExceptionsTest.test(1200);
 		MD5Test.test(1300);
		MethodOverloadingTest.test(1400);
		RuntimeExceptionsTest.test(1500);
		InfusionTest.test(1600);
	}

	/*
	private short max(short a, short b)
	{
		short c,d,e;
		
		d = 300;
		e = 10;
		c = (short)(d + e);
		int f = ((d + e) * a) / b;
		c = (short)(((d + e) * a) / b);
		
		if (d+e>a) return a;
		
		return a>b?a:b;
	}
	*/

	private static class TestRunner implements Runnable
	{

		public void run()
		{
			try {
				test();
			} catch (OutOfMemoryError err)
			{
				// out of memory
			} catch (StackOverflowError err)
			{
				// stack overflow
			}
		}
	}
	
	public static void stressTest()
	{
		for (int i=0; i<10; i++)
		{
			Thread t = new Thread(new TestRunner());
			t.start();
			Thread.sleep(500);
		}
	}
	
	public static int recurseTree(TreeNode node, short i, byte[] targetArray)
	{
		short ret = 0;
		if (node==null) return 0;
		
		ret += recurseTree(node.getLeft(), i, targetArray);
		
		targetArray[i+ret] = node.getValue();
		ret++;
		
		ret += recurseTree(node.getRight(), (short)(i + ret), targetArray);
		
		return ret;
	}

	public static void treeSortTest()
	{

		// int numbers[] = new int[] { 21,6,36,76,7,97,94,30,90,86,13,80,84,79,28,55,36,95,23,82,57,73,28,46,48,94,18,23,86,100,47,42,39,33,52,98,77,81,86,64,27,70,91,42,6,95,8,6,36,71,79,32,27,34,87,100,85,90,69,12,73,70,76,65,51,21,4,5,82,77,63,87,12,11,69,79,12,35,43,35,67,16,38,78,60,6,79,92,43,69,57,74,58,21,45,69,45,55,73,24 };
		// int numbers[] = new int[] { 21,6,36,76,7,97,94,30,90,86,13,80,84,79,28,55,36,95,23 };
		byte numbers[] = new byte[] { 
				21,  6, 36, 76,  7, 97, 94, 30, 90, 86,
				13, 80, 84, 79, 28, 55, 36, 95, 23, 82
				};		
		TreeNode rootNode = new TreeNode(numbers[0]);
		
		for (short i=1; i<numbers.length; i++)
			rootNode.insert(new TreeNode(numbers[i]));
		
		// should leave tree intact since we have a ref to the rootnode
		// in the current frame
		recurseTree(rootNode, (short)0, numbers);
		
		boolean isAscending = true;
		for (short i=0; i<numbers.length-1; i++)
			isAscending &= numbers[i]<=numbers[i+1];
		
	}
	
	public static short max(short a, short b)
	{
		byte c = 3;
		byte d = (byte)(a + b + c);
		
	  return a>b?a:b;
	}

	static Object lock = new Object(); 

	public static void main(String[] args)
	{
		// test();
		// stressTest();
		
		
		for (short i=0; i<3; i++)
			new Thread() {
				public void run()
				{
					for (short j=0; j<5; j++) 
					{
						synchronized(lock) 
						{ 
							treeSortTest();
						}
						Thread.sleep(10);
					}
				}
			}.start();
		
	}
}
