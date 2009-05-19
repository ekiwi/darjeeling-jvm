package javax.util;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class ArrayBag<E> implements Bag<E>
{
	
	private short size;
	private short growSize;
	private Object elements[];

	public ArrayBag(short growSize)
	{
		this.growSize = growSize;
	}
	
	public ArrayBag()
	{
		this((short)8);
	}
	
	public void add(E element)
	{
		// lazily construct the element array if none was created
		if (elements==null) elements = new Object[growSize];
		
		// check if we need to grow the array
		if (size>=elements.length)
		{
			Object[] newElements = new Object[size+growSize];
			// TODO use System.arrayCopy here
			for (int i=0; i<elements.length; i++)
				newElements[i] = elements[i];
			elements = newElements;
		}
		
		// add the new element
		elements[size] = element;
		size++;
	}

	
	public void clear()
	{
		size = 0;
		elements = null;
	}

	
	public boolean contains(E element)
	{
		for (int i=0; i<size; i++)
			if (elements[i]==element) return true;
		return false;
	}

	
	public boolean isEmpty()
	{
		return size==0;
	}

	
	public E get(short index)
	{
		if ((index<0)||(index>=size))
			throw new IndexOutOfBoundsException();
		
		return (E)elements[index];
	}

	public void remove(short index)
	{
		if ((index<0)||(index>=size))
			throw new NoSuchElementException();
		size--;
		if (size>0) elements[index] = elements[size];
		return;
	}
	
	
	public void remove(E element)
	{
		for (int i=0; i<size; i++)
			if (elements[i]==element)
			{
				size--;
				elements[i] = elements[size];
				return;
			}
		throw new NoSuchElementException();
	}

	
	public short size()
	{
		return size;
	}

	
	public Object[] toArray()
	{
		Object[] ret = new Object[size];
		// TODO System.arrayCopy
		for (int i=0; i<size; i++) ret[i] = elements[i];
		return ret;
	}
	
	
	public Iterator<E> iterator()
	{
		return new ArrayBagIterator();
	}
	
	private class ArrayBagIterator implements Iterator<E>
	{
		private short index = 0;
		
		public boolean hasNext()
		{
			return index<size;
		}

		public E next()
		{
			E ret = (E)elements[index]; 
			index++;
			return ret;
		}

		public void remove()
		{
		}
	}

}
