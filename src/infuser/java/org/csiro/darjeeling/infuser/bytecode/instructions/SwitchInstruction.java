/*
 *	SwitchInstruction.java
 * 
 *	Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
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
package org.csiro.darjeeling.infuser.bytecode.instructions;

import org.csiro.darjeeling.infuser.bytecode.Opcode;

public abstract class SwitchInstruction extends BranchInstruction
{

	protected int[] switchAddresses;

	public SwitchInstruction(Opcode opcode, int branchAdress)
	{
		super(opcode, branchAdress);
	}
	
	public int[] getSwitchAddresses()
	{
		return switchAddresses;
	}
	
	public void setSwitchAddress(int index, int address)
	{
		this.switchAddresses[index] = address;
	}
	

}
