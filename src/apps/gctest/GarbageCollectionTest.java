public class GarbageCollectionTest //implements Runnable
{
	
	public static int recurseTree(TreeNode node, int i, byte[] targetArray)
	{
		int ret = 0;
		if (node==null) return 0;
		
		ret += recurseTree(node.getLeft(), i, targetArray);
		
		targetArray[i+ret] = node.getValue();
		ret++;
		
		ret += recurseTree(node.getRight(), i + ret, targetArray);
		
		return ret;
	}

	public static void treeSortTest(int testBase)
	{

		byte numbers[] = new byte[] { 
				21,  6, 36, 76,  7, 97, 94, 30, 90, 86,
				13, 80, 84, 79, 28, 55, 36, 95, 23, 82,
				57, 73, 28, 46, 48, 94, 18, 23, 86,100,
				47, 42, 39, 33, 52, 98, 77, 81, 86, 64,
				27, 70, 91, 42,  6, 95,  8,  6, 36, 71,
				32, 27, 34, 87,100, 85, 90, 69, 12, 73,
				70, 76, 65, 51, 21,  4,  5, 82, 77, 63,
				87, 12, 11, 69, 79, 12, 35, 43, 35, 67,
				16, 38, 78, 60,  6, 79, 92, 43, 69, 57,
				74, 58, 21, 45, 69, 45, 55, 73, 24, 79,
				};
		
		
		TreeNode rootNode = new TreeNode(numbers[0]);
		
		for (int i=1; i<numbers.length; i++)
			rootNode.insert(new TreeNode(numbers[i]));
		
		// should leave tree intact since we have a ref to the rootnode
		// in the current frame
		Darjeeling.gc();
		recurseTree(rootNode, 0, numbers);
		
		boolean isAscending = true;
		for (int i=0; i<numbers.length-1; i++)
			isAscending &= numbers[i]<=numbers[i+1];
		
//		Darjeeling.assertTrue(testBase + 0, isAscending);
		
	}
	
	public static void main(String[] args)
	{
//		for (int i=0; i<3; i++)
//		treeSortTest(10 + i);
		treeSortTest(0);
	}
	
}
