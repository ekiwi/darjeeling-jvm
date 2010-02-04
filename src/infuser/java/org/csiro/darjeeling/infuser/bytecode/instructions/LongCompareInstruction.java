package org.csiro.darjeeling.infuser.bytecode.instructions;

import org.csiro.darjeeling.infuser.bytecode.InstructionHandle;
import org.csiro.darjeeling.infuser.bytecode.Opcode;
import org.csiro.darjeeling.infuser.structure.BaseType;

public class LongCompareInstruction extends SimpleInstruction
{

	public LongCompareInstruction(Opcode opcode)
	{
		super(opcode);
	}
	
	@Override
	public BaseType getLogicalOutputType(int index, InstructionHandle handle)
	{
		return BaseType.Byte;
	}

}
