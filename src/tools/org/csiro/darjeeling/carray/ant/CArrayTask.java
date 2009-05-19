/*
 *	CArrayTask.java
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

package org.csiro.darjeeling.carray.ant;

import java.io.* ;

import org.apache.tools.ant.*;

public class CArrayTask extends Task
{
	private final static int LINESIZE = 16;
	private String src, dest, arrayName;
	private boolean progmemKeyword = false;
	private boolean constKeyword = true;
	
	public void execute()
	{
		byte[] bytes;
		
		// make sure properties are set
		if (src==null)
			throw new BuildException("Property 'src' not set");
		if (dest==null)
			throw new BuildException("Property 'dest' not set");



        long srcDate =new File(src ).lastModified();
        long destDate=new File(dest).lastModified();
        if(destDate > srcDate)
        {
            log("This file is up to date",Project.MSG_VERBOSE);
            return ;
        }

        
		// open input file
		try {
			FileInputStream fileInput = new FileInputStream(src);
			bytes = new byte[fileInput.available()];
			fileInput.read(bytes);
			fileInput.close();
		} catch (FileNotFoundException fnfex)
		{
			throw new org.apache.tools.ant.BuildException("File not found: " + src);
		} catch (IOException ioex) {
			throw new org.apache.tools.ant.BuildException("IO error while reading: " + src);
		}

		log("Converting "+src+" to "+dest+", "+bytes.length+" bytes",Project.MSG_INFO);
		
		// write C-style array definition
		try {
			FileOutputStream fout = new FileOutputStream(dest);
			PrintWriter writer = new PrintWriter(fout);
			writeArray(writer, bytes);
			writer.flush();
			writer.close();
			fout.close();
		} catch (IOException ioex) {
			throw new org.apache.tools.ant.BuildException("IO error while writing: " + src);
		}
		
	}
	
	private void writeArray(PrintWriter out, byte[] bytes)
	{
		String name = arrayName;
		if (name==null) name = src;
		
		out.printf("#ifndef __%s_di__\n", name);	
		out.printf("#define __%s_di__\n", name);	
		
		out.printf(
				"%sunsigned char %s%s[] = {\n", 
				constKeyword ? "const ":"", 
				progmemKeyword ? "PROGMEM ":"", 
				name
			);
		
		int left = bytes.length;
		int pos = 0;
		
		while (left>0)
		{
			int lineLength = Math.min(left, LINESIZE);
			out.print("\t");
			for (int i=0; i<lineLength; i++)
			{
				out.printf("0x%02x, ", bytes[pos]);
				pos++;
			}
			out.print("\n");
			left-=lineLength;
		}
		
		out.printf("};\n\n");

		out.printf("#endif\n\n");
		
	}
	
	public void setSrc(String src)
	{
		this.src = src;
	}

	public void setDest(String dest)
	{
		this.dest = dest;
	}
	
	public void setArrayname(String arrayName)
	{
		this.arrayName = arrayName;
	}
	
	public void setProgmem(boolean value)
	{
		progmemKeyword = value;
	}

	public void setConst(boolean value)
	{
		constKeyword = value;
	}

}
