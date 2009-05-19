/*
 *	FieldList.java
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
package org.csiro.darjeeling.infuser.structure;

import java.util.ArrayList;
import java.util.Collection;

import org.csiro.darjeeling.infuser.structure.elements.AbstractField;

/**
 * 
 * List of fields inside a class.
 * 
 * @author Niels Brouwers
 *
 */
public class FieldList
{

	// field list
	private ArrayList<AbstractField> fields;
	
	/**
	 * Creates a new FieldList
	 */
	public FieldList()
	{
		fields = new ArrayList<AbstractField>();
	}
	
	public void addField(AbstractField field)
	{
		fields.add(field);
	}
	
	public AbstractField getFieldByName(String name)
	{
		for (AbstractField field : fields)
			if (field.getName().equals(name))
				return field;
		
		return null;
	}

	public Collection<AbstractField> getFields()
	{
		return fields;
	}
	
}
