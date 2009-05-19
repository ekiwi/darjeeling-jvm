package testvm.classes;

import javax.util.ArrayBag;

public class SinkList extends ArrayBag<Sink>
{
	
	public Sink getSinkByNodeId(short nodeId)
	{
		Sink ret;
		
/*
		// check if the sink already exists in our list
		for (short i=0; i<size(); i++)
			if ((ret=get(i)).nodeId==nodeId)
				return ret;
	
		*/
		// if not, add a new entry and return it
//		ret = new Sink(nodeId);
		// add(ret);
		ret = null;
		return ret;
	}

}
