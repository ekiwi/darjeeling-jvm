/*
 *	FieldMapVisitor.java
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
package org.csiro.infuser.processingphase;

import org.csiro.darjeeling.infuser.structure.DescendingVisitor;
import org.csiro.darjeeling.infuser.structure.Element;
import org.csiro.darjeeling.infuser.structure.elements.AbstractClassDefinition;
import org.csiro.darjeeling.infuser.structure.elements.AbstractField;
import org.csiro.darjeeling.infuser.structure.elements.AbstractStaticFieldList;

public class FieldMapVisitor extends DescendingVisitor
{
	
	//@Override
	public void visit(AbstractClassDefinition element)
	{
		int nonRefOffset = 0;
		int refOffset = 0;
		
		AbstractClassDefinition superClass = element.getSuperClass();
		if (superClass!=null)
		{
			nonRefOffset += superClass.getNonRefSize();
			refOffset += superClass.getNrRefs();
		}
		
		for (AbstractField field : element.getFieldList().getFields())
		{
			if (field.isRef())
			{
				field.setOffset(refOffset);
				refOffset++;
			} else
			{
				field.setOffset(nonRefOffset);
				nonRefOffset += field.getSize();
			}
		}
	}
	
	//@Override
	public void visit(AbstractStaticFieldList element)
	{
		// TODO: use the BaseType enum here
		int counters[] = new int[4];
		
		for (AbstractField field : element.getChildren())
		{
			int counterIndex;
			switch (field.getSize())
			{
				case 1: counterIndex = 0; break;
				case 2: counterIndex = 1; break;
				case 4: counterIndex = 2; break;
				case -1: counterIndex = 3; break;
				default:
					throw new IllegalStateException("Unhandled field size: " + field.getSize());
			}
			
			field.getGlobalId().setEntityId(counters[counterIndex]);
			counters[counterIndex]++;
		}
		
		element.setNrBytes(counters[0]);
		element.setNrShorts(counters[1]);
		element.setNrInts(counters[2]);
		element.setNrRefs(counters[3]);
	
	}
	
	//@Override
	public void visit(Element element)
	{
	}

}
