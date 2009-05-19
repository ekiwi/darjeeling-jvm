package javax.util;

import java.util.Collection;

/**
 * 
 * A <i>bag</i> or <i>multiset</i> is a collection where each element can appear more than one time, and where the order of
 * elements doesn't matter. 
 * <p>
 * This interface adds nothing to the Collection interface, but simply acts as a way to further group implementations.    
 * 
 * @author Niels Brouwers
 *
 */
public interface Bag<E> extends Collection<E>
{
	
	public E get(short index);

}
