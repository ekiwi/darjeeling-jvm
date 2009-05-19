public class TreeNode
{
	
	private TreeNode left,right;
	private byte value;
	
	public TreeNode(byte value)
	{
		right = left = null;
		this.value = value;
	}
	
	public byte getValue()
	{
		return value;
	}
	
	public void insert(TreeNode node)
	{
		if (node.getValue()<value)
			insertLeft(node);
		else
			insertRight(node);
	}
	
	private void insertLeft(TreeNode node)
	{
		if (left==null)
			left=node;
		else
			left.insert(node);
	}
	
	private void insertRight(TreeNode node)
	{
		if (right==null)
			right=node;
		else
			right.insert(node);
	}
	
	public TreeNode getLeft()
	{
		return left;
	}

	public TreeNode getRight()
	{
		return right;
	}

}
