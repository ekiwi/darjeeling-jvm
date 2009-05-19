package javax.darjeeling.vm;


public class Infusion
{
	
	private Object internalInfusion;
	
	private Infusion(Object internalInfusion)
	{
		this.internalInfusion = internalInfusion;
	}
	
	public static native short getInfusionCount();
	
	private static native Object _getInfusion(short index);
	
	public static Infusion getInfusion(short index)
	{
		// check range
		if (index<0||index>getInfusionCount())
			throw new IndexOutOfBoundsException();
		
		// create new instance
		return new Infusion(_getInfusion(index));
	}
	
	private static native String _getName(Object internalInfusion);	
	
	public String getName()
	{
		return _getName(internalInfusion);		
	}
	
	private static native short _getImportedInfusionCount(Object internalInfusion);
	
	public short getImportedInfusionCount()
	{
		return _getImportedInfusionCount(internalInfusion);
	}
	
	private static native Object _getImportedInfusion(Object internalInfusion, short index);
	
	public Infusion getImportedInfusion(short index)
	{
		// check range
		if (index<0||index>_getImportedInfusionCount(internalInfusion))
			throw new IndexOutOfBoundsException();
		
		return new Infusion(_getImportedInfusion(internalInfusion, index));
	}
	
	public static Infusion getInfusionByName(String name)
	{
		for (short i=0; i<getInfusionCount(); i++)
		{
			Object internalInfusion = _getInfusion(i);
			if (_getName(internalInfusion).equals(name))
				return new Infusion(internalInfusion);
		}		
		
		return null;
	}
	
	private native void _unload(Object internalInfusion) throws InfusionUnloadDependencyException;
	
	public void unload() throws InfusionUnloadDependencyException
	{
		_unload(internalInfusion);
	}

}
