/*
 *	DummyHandle.java
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
package org.csiro.darjeeling.infuser.bytecode;

import org.csiro.darjeeling.infuser.structure.BaseType;

public class DummyHandle extends InstructionHandle
{
	
	private InstructionHandle handle;
	
	public DummyHandle(InstructionHandle handle)
	{
		super(null);
		this.handle = handle;
	}
	
	public DummyHandle()
	{
		this(null);
	}
	
	@Override
	public int getPc()
	{
		if (handle==null) 
			return -1;
		else
			return handle.getPc();
	}

	@Override
	public BaseType getOutputType(int index)
	{
		return BaseType.Ref;
	}
	
	@Override
	public BaseType getLogicalOutputType(int index)
	{
		return BaseType.Ref;
	}
	
}
