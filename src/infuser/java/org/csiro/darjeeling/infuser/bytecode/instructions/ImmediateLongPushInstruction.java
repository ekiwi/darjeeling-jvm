package org.csiro.darjeeling.infuser.bytecode.instructions;

import java.io.DataOutputStream;
import java.io.IOException;

import org.csiro.darjeeling.infuser.bytecode.InstructionHandle;
import org.csiro.darjeeling.infuser.bytecode.Opcode;
import org.csiro.darjeeling.infuser.structure.BaseType;

public class ImmediateLongPushInstruction extends PushInstruction
{

	public ImmediateLongPushInstruction(Opcode opcode, long value)
	{
		super(opcode, value);
	}

	@Override
	public void dump(DataOutputStream out) throws IOException
	{
		out.write(opcode.getOpcode());
		out.writeLong(value);
	}
	
	@Override
	public BaseType getLogicalOutputType(int index, InstructionHandle handle)
	{
		return BaseType.Long;
	}

	@Override
	public int getLength()
	{
		return 9;
	}

}
