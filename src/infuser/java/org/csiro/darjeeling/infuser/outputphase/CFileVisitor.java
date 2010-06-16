/*
 * CFileVisitor.java
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
 
package org.csiro.darjeeling.infuser.outputphase;

import java.io.PrintWriter;

import org.apache.bcel.generic.Type;
import org.csiro.darjeeling.infuser.structure.DescendingVisitor;
import org.csiro.darjeeling.infuser.structure.Element;
import org.csiro.darjeeling.infuser.structure.ParentElement;
import org.csiro.darjeeling.infuser.structure.elements.AbstractHeader;
import org.csiro.darjeeling.infuser.structure.elements.AbstractMethodImplementation;
import org.csiro.darjeeling.infuser.structure.elements.internal.InternalInfusion;
import org.csiro.darjeeling.infuser.structure.elements.internal.InternalMethodImplementation;

public class CFileVisitor extends DescendingVisitor
{
	private PrintWriter writer;
	
	public CFileVisitor(PrintWriter writer)
	{
		this.writer = writer;
	}
	
	@Override
	public void visit(InternalInfusion element)
	{
		AbstractHeader header = element.getHeader();
        writer.println("/*");
        writer.println(" * This file is machine-generated by the infuser tool.");
        writer.println(" * Do not edit.");
        writer.println(" */");

		writer.println("#include \"types.h\"");
		writer.println("");
		
		visit((ParentElement<Element>)element);
		
		// generate switch code
		writer.println(String.format("void %s_native_handler(dj_global_id id)", header.getInfusionName()));
		writer.println("{");
		writer.println("\tswitch(id.entity_id)");
		writer.println("\t{");
		
		for (AbstractMethodImplementation methodImplementation : element.getMethodImplementationList().getChildren())
		{
			if (((InternalMethodImplementation)methodImplementation).isNative())
			{
				writer.printf("\t\tcase %d: %s(); break;\n",
						methodImplementation.getGlobalId().getEntityId(),
						generateMethodName(methodImplementation)
						);
			}
		}
		
		writer.println("\t}");
		writer.println("}");
		writer.println("");
	}
	
	private String generateMethodName(AbstractMethodImplementation element)
	{
		String className = element.getParentClass().getName();
		Type returnType = Type.getReturnType(element.getMethodDefinition().getSignature());
		Type argTypes[] = Type.getArgumentTypes(element.getMethodDefinition().getSignature());
		className = className.replaceAll("\\.", "_").replaceAll("\\$", "_inner_");
		String descr = returnType + "_" + element.getMethodDefinition().getName();
		for (Type argType : argTypes) descr += "_" + argType.toString();
		descr = descr.toString().replaceAll("\\p{Punct}", "_");
		return String.format("%s_%s",
				className,
				descr
				);
	}
	
	private String generateFriendlyMethodName(AbstractMethodImplementation element)
	{
		String className = element.getParentClass().getName();
		Type returnType = Type.getReturnType(element.getMethodDefinition().getSignature());
		Type argTypes[] = Type.getArgumentTypes(element.getMethodDefinition().getSignature());
		String args = "";
		for (Type argType : argTypes) args += ((args.length()==0)?"":", ") + argType; 

		// write out comment describing the method in a readable form
		return String.format("%s %s.%s(%s)",
				returnType.toString(),
				className,
				element.getMethodDefinition().getName(),
				args
				);
	}
	
	public void visit(InternalMethodImplementation element)
	{
		if (element.isNative()==null)
		{
			Type argTypes[] = Type.getArgumentTypes(element.getMethodDefinition().getSignature());
			String args = "";
			for (Type argType : argTypes) args += ((args.length()==0)?"":", ") + argType; 

			// write out comment describing the method in a readable form
			writer.printf("// %s\n", generateFriendlyMethodName(element));

			// declare method
			writer.printf("void %s();\n\n", generateMethodName(element));
			
		}

	}
	
	public void visit(Element element)
	{
	}

}
