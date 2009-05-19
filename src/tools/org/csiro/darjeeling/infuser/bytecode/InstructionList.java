/*
 *	InstructionList.java
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
package org.csiro.darjeeling.infuser.bytecode;

import java.util.ArrayList;
import java.util.Collection;

import org.csiro.darjeeling.infuser.bytecode.instructions.BranchInstruction;
import org.csiro.darjeeling.infuser.bytecode.instructions.SwitchInstruction;

public class InstructionList
{
	
	private ArrayList<InstructionHandle> instructions;
	
	public InstructionList()
	{
		instructions = new ArrayList<InstructionHandle>();
	}
	
	private InstructionList(ArrayList<InstructionHandle> instructions)
	{
		this.instructions = instructions;
	}
	
	public Collection<InstructionHandle> getInstructionHandles()
	{
		return instructions;
	}
	
	public void addInstructionHandle(InstructionHandle handle)
	{
		instructions.add(handle);
	}
	
	public int size()
	{
		return instructions.size();
	}
	
	public InstructionHandle get(int index)
	{
		return instructions.get(index);
	}

	public InstructionHandle getHandleByPc(int pc)
	{
		for (InstructionHandle handle : instructions)
			if (handle.getPc()==pc) return handle;
		
		throw new IllegalArgumentException("No handle at pc=" + pc);
	}

	public void insertBefore(InstructionHandle handle, InstructionHandle newHandle)
	{
		int index = instructions.indexOf(handle);
		if (index<0)
			throw new IllegalArgumentException("Handle not found for insertion: " + handle);

		if (index>0)
		{
			InstructionHandle previousHandle = instructions.get(index-1);
			previousHandle.setNextHandle(newHandle);
			newHandle.addIncomingHandle(previousHandle);
		}

		instructions.add(index, newHandle);
		newHandle.setNextHandle(handle);
	}

	public void insertBefore(InstructionHandle handle, Collection<InstructionHandle> newHandles)
	{
		for (InstructionHandle newHandle : newHandles)
			insertBefore(handle, newHandle);
	}
	
	public void insertAfter(InstructionHandle handle, InstructionHandle newHandle)
	{
		int index = instructions.indexOf(handle);
		if (index<0)
			throw new IllegalArgumentException("Handle not found for insertion: " + handle);
		
		if (index<instructions.size()-1)
		{
			InstructionHandle nextHandle = instructions.get(index+1);
			newHandle.setNextHandle(nextHandle);
		}
		
		handle.setNextHandle(newHandle);

		instructions.add(index+1, newHandle);
		newHandle.addIncomingHandle(handle);
		
	}
	
	public void remove(InstructionHandle handle)
	{
		assert(handle.getOutgoingHandles().size()==1) : "Can only remove non-branch and non-return handles";
		InstructionHandle nextHandle = handle.getOutgoingHandles().get(0);
		
		// remap any incoming handles to point to the next instruction
		for (InstructionHandle incomingHandle : handle.getIncomingHandles())
			incomingHandle.replaceOutgoingHandle(handle, nextHandle);
		
		// remove the instruction itself
		instructions.remove(handle);
		
	}
	
	public void threadStates()
	{
		for (int i=0; i<instructions.size(); i++)
		{
			InstructionHandle handle = instructions.get(i);
			Instruction instruction = handle.getInstruction();
			Opcode opcode = instruction.getOpcode();

			// do normal forward threading. This code establishes a pre/post link
			// between two consecutive instructions. Note that this should not 
			// be applied in the case of an unconditional JUMP instruction, 
			// in the case of a throw instruction, in the case of a return 
			// instruction, or in the case of a switch.
			if ((!opcode.isReturn()) && 
				(!opcode.isUnConditionalBranch()) &&
				(!opcode.isThrow()) &&
				(!opcode.isSwitch())
				)
			{
				InstructionHandle nextHandle = instructions.get(i+1);
				handle.setNextHandle(nextHandle);
				nextHandle.addIncomingHandle(handle);
			}

			// thread switch instructions
			if (opcode.isSwitch())
			{
				SwitchInstruction switchInstruction = (SwitchInstruction)instruction;
				for (int switchAddress : switchInstruction.getSwitchAddresses())
				{
					InstructionHandle target = getHandleByPc(switchAddress);
					handle.addSwitchTarget(target);
					target.addIncomingHandle(handle);
				}
			}
			
			// in case of jump instructions, create a pre/post link to the
			// jump adress
			if (opcode.isBranch())
			{
				InstructionHandle target = getHandleByPc(((BranchInstruction) instruction).getBranchAdress());
				handle.setBranchHandle(target);
				target.addIncomingHandle(handle);
			}
			
		}
	}
	
	public void reThreadStates()
	{
		// clear incoming handles
		for (InstructionHandle handle : instructions)
			handle.clearIncomingHandles();
		
		// re-thread the instruction block
		for (int i=0; i<instructions.size(); i++)
		{
			InstructionHandle handle = instructions.get(i);
			Instruction instruction = handle.getInstruction();
			Opcode opcode = instruction.getOpcode();
			
			handle.getIncomingHandles().clear();

			// do normal forward threading. This code establishes a pre/post link
			// between two consecutive instructions. Note that this should not 
			// be applied in the case of an unconditional JUMP instruction, 
			// in the case of a throw instruction, in the case of a return 
			// instruction, or in the case of a switch.
			if ((!opcode.isReturn()) && 
				(!opcode.isUnConditionalBranch()) &&
				(!opcode.isThrow()) &&
				(!opcode.isSwitch())
				)
			{
				InstructionHandle nextHandle = instructions.get(i+1);
				handle.setNextHandle(nextHandle);
				nextHandle.addIncomingHandle(handle);
			}
			
			// thread switch instructions
			if (opcode.isSwitch())
				for (InstructionHandle switchTarget : handle.getSwitchTargets())
					switchTarget.addIncomingHandle(handle);
				
			// in case of jump instructions, create a pre/post link to the
			// jump adress
			if (opcode.isBranch())
				handle.getBranchHandle().addIncomingHandle(handle);
			
		}
	}
	
	public void fixBranchAddresses()
	{
		int pc = 0;
		for (InstructionHandle handle : instructions)
		{
			handle.setPc(pc);
			pc += handle.getInstruction().getLength();
		}

		for (InstructionHandle handle : instructions)
		{
			Opcode opcode = handle.getInstruction().getOpcode();
			
			if (opcode.isBranch())
			{
				BranchInstruction branch = (BranchInstruction)handle.getInstruction();
				branch.setBranchAdress(handle.getBranchHandle().getPc() - handle.getPc());
			}
			
			if (opcode.isSwitch())
			{
				SwitchInstruction switchInstruction = (SwitchInstruction)handle.getInstruction();
				for (int i=0; i<handle.getSwitchTargets().size(); i++)
					switchInstruction.setSwitchAddress(i, handle.getSwitchTargets().get(i).getPc() - handle.getPc());
			}
		}
	}	
	
	@SuppressWarnings("unchecked")
	public Object clone() throws CloneNotSupportedException
	{
		return new InstructionList((ArrayList<InstructionHandle>)instructions.clone()); 
	}

}
