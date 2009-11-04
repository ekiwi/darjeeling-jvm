package org.csiro.messaging;

public class ArrayQueue<T> {

	protected final T[] queue;
	protected byte head, size;

	public ArrayQueue(int size) {
		queue = (T[]) (new Object[size]);
		head = 0;
		size = 0;
	}

	public boolean offer(T element) {
		if (size >= queue.length)
			return false;

		queue[(head + size) % queue.length] = element;
		size++;

		return true;
	}

	public T peek() {
		if (size == 0)
			return null;
		else
			return queue[head];
	}

	public T peek(short index) {
		if (index >= size)
			throw new IndexOutOfBoundsException();
		else
			return queue[(head + index) % queue.length];
	}

	public T poll() {
		if (size == 0)
			return null;
		else {
			T ret = queue[head];
			queue[head] = null;
			head = (byte) ((head + 1) % queue.length);
			size--;
			return ret;
		}
	}

	public void remove() {
		if (size > 0) {
			queue[head] = null;
			head = (byte) ((head + 1) % queue.length);
			size--;
		}
	}

	public short numElements() {
		return size;
	}

}
