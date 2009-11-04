/*
 *	DescendingVisitor.java
 * 
 *	Copyright (c) 2008-2009 CSIRO, Delft University of Technology.
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
package org.csiro.darjeeling.infuser.structure;

import org.csiro.darjeeling.infuser.structure.elements.AbstractInfusion;
import org.csiro.darjeeling.infuser.structure.elements.AbstractReferencedInfusionList;

public abstract class DescendingVisitor extends ElementVisitor
{

	public DescendingVisitor()
	{
	}
	
	@Override
	public void visit(AbstractReferencedInfusionList element)
	{
		visit((ParentElement<AbstractInfusion>)element);
	}
	
	public <T extends Element> void visit(ParentElement<T> element)
	{
		for (Element child : element.getChildren())
		{
			child.accept(this);
		}
	}	

}
