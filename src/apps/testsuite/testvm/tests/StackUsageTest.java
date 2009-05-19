package testvm.tests;

import javax.darjeeling.Darjeeling;

import testvm.classes.TreeNode;

public class StackUsageTest implements Runnable
{
	
	private Object lock;
    private int testBase;

    private static int threads;
	
	public StackUsageTest(Object lock,int testBase)
	{
		this.lock = lock;
        this.testBase = testBase;
	}

	public static int recurseTree(TreeNode node, short i, byte[] targetArray)
	{
		int ret = 0;
		if (node==null) return 0;
		
		ret += recurseTree(node.getLeft(), i, targetArray);
		
		targetArray[i+ret] = node.getValue();
		ret++;
		
		ret += recurseTree(node.getRight(), (short)(i + ret), targetArray);
		
		return ret;
	}

	public void run()
	{
		synchronized(lock)
		{
            Darjeeling.assertTrue(testBase+0, true);
            byte numbers[] = new byte[] { 21,6,36,76,7,97,94,30,90,86,13,80,84,79,28,55,36,95,23 };
			
			TreeNode rootNode = new TreeNode(numbers[0]);
			
			for (int i=1; i<numbers.length; i++)
			{
				rootNode.insert(new TreeNode(numbers[i]));
			}
			
			// should leave tree intact since we have a ref to the rootnode
			// in the current frame
			recurseTree(rootNode, (short)0, numbers);
			
			boolean isAscending = true;
			for (short i=0; i<numbers.length-1; i++)
				isAscending &= numbers[i]<=numbers[i+1];
				
			threads--;
            Darjeeling.assertTrue(testBase+1, true);
		}
        Darjeeling.assertTrue(testBase+2, true);
		while (threads>0);
        Darjeeling.assertTrue(testBase+3, true);

	}

	public static void test(int testBase)
	{
		Object lock = new Object();
		for (int i=0; i<3; i++)
		{
			threads++;
			Thread t = new Thread(new StackUsageTest(lock,testBase+i*10));
			t.start();
		}

        boolean done=false;
        while(!done)
        {
            synchronized(lock)
            {
                done = (threads == 0);
            }
        }

        Darjeeling.assertTrue(testBase+99,true);
        
	}

}
