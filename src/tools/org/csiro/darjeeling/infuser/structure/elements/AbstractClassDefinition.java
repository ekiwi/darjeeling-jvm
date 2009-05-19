/*
 *	AbstractClassDefinition.java
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
package org.csiro.darjeeling.infuser.structure.elements;

import java.util.Collection;
import java.util.HashMap;

import org.csiro.darjeeling.infuser.structure.ElementId;
import org.csiro.darjeeling.infuser.structure.ElementVisitor;
import org.csiro.darjeeling.infuser.structure.FieldList;
import org.csiro.darjeeling.infuser.structure.GlobalId;
import org.csiro.darjeeling.infuser.structure.ParentElement;

public abstract class AbstractClassDefinition extends ParentElement<AbstractMethod>
{
	
	// list of interfaces implemented by this class
	protected HashMap<String, AbstractClassDefinition> interfaces;
	
	// super class
	protected AbstractClassDefinition superClass;
	
	// class fields
	protected FieldList fieldList;
	
	// class assigned global ID
	protected GlobalId globalId;
	
	// class name and superclass' name
	protected String className, superClassName;
	
	// for error reporting
	protected String fileName;
	
	protected AbstractClassDefinition()
	{
		super(ElementId.CLASSDEF);
		fieldList = new FieldList();
		interfaces = new HashMap<String, AbstractClassDefinition>();
	}
	
	public AbstractMethod getMethodByDef(AbstractMethodDefinition methodDef)
	{
		for (AbstractMethod method : getChildren())
			if (AbstractMethodDefinition.getComparator().compare(method.getMethodDef(),methodDef)==0)
				return method;
		
		return null;
	}
	
	public AbstractField getFieldByName(String name)
	{
		AbstractField ret = null;
		
		ret = fieldList.getFieldByName(name);
		if ((ret==null)&&(superClass!=null)) ret = superClass.getFieldByName(name);
		
		return ret;
	}

	public void accept(ElementVisitor visitor)
	{
		visitor.visit(this);
	}

	public void setGlobalId(GlobalId globalId)
	{
		this.globalId = globalId;
	}
	
	public GlobalId getGlobalId()
	{
		return globalId;
	}
	
	/**
	 * @return the className
	 */
	public String getName()
	{
		return className;
	}

	/**
	 * @return the superClassName
	 */
	public String getSuperClassName()
	{
		return superClassName;
	}
	
	/**
	 * @return the superClass
	 */
	public AbstractClassDefinition getSuperClass()
	{
		return superClass;
	}

	/**
	 * @param superClass the superClass to set
	 */
	public void setSuperClass(AbstractClassDefinition superClass)
	{
		this.superClass = superClass;
	}
	
	public FieldList getFieldList()
	{
		return fieldList;
	}
	
	public int getNonRefSize()
	{
		int ret = 0;

		if (superClass!=null)
			ret += superClass.getNonRefSize();
		
		for (AbstractField field : fieldList.getFields())
			if (!field.isRef())
				ret += field.getSize();
		return ret;
	}
	
	public int getNrRefs()
	{
		int ret = 0;
		
		if (superClass!=null)
			ret += superClass.getNrRefs();
		
		for (AbstractField field : fieldList.getFields())
			if (field.isRef()) ret ++;
		
		return ret;
	}
	
	public Collection<String> getInterfaceNames()
	{
		return interfaces.keySet();
	}
	
	public HashMap<String, AbstractClassDefinition> getInterfaces()
	{
		return interfaces;
	}
	
	public String getFileName()
	{
		return fileName;
	}

}
