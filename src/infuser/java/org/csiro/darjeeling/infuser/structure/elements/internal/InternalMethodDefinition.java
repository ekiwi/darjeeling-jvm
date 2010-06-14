/*
 * InternalMethodDefinition.java
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
package org.csiro.darjeeling.infuser.structure.elements.internal;

import org.apache.bcel.classfile.Method;
import org.csiro.darjeeling.infuser.structure.ElementVisitor;
import org.csiro.darjeeling.infuser.structure.elements.AbstractMethodDefinition;

public class InternalMethodDefinition extends AbstractMethodDefinition
{
	
	public InternalMethodDefinition(String methodName, String descriptor)
	{
		super(methodName, descriptor);
	}

	public static InternalMethodDefinition fromMethod(Method method)
	{
		return new InternalMethodDefinition(method.getName(), method.getSignature()); 
	}
	
	public void accept(ElementVisitor visitor)
	{
		visitor.visit(this);
	}	
}
