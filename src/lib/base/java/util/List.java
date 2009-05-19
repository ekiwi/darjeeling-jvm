package java.util;

public interface List<E> extends Collection<E>
{

	public E get(short index);
	public E set(short index, E element);
	
	public E remove(short index);
	public short indexOf(E element);

}
