package java.util;

public interface Collection<E> extends Iterable<E>
{
	
	public void add(E element);
	public void remove(E element);

	public short size();
	
	public void clear();
	
	public boolean contains(E element);
	public boolean isEmpty();
	
	public Object[] toArray();

}
