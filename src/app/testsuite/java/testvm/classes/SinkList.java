/*
 *	SinkList.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
package testvm.classes;


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
