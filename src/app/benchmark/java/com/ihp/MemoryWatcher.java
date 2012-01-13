/**
 * 
 */
package com.ihp;

/**
 * @author Michael Maaser
 *
 */
public class MemoryWatcher extends Thread {

	private boolean stopped = false;
	private Runtime myRuntime;
	private long maxFree;
	private long minFree;
	private long base;
	private Object lock;

	public MemoryWatcher() {
		myRuntime = Runtime.getRuntime();
		lock = new Object();
		reset();
	}
	
	public void run() {
		while (!stopped ) {
			measure();
			try {
				Thread.sleep(40);
			} catch (Exception e) {
			}
		}
	}
	
	public void explicitGC() {
		myRuntime.gc();
	}
	
	public void measure() {
		synchronized(lock) {
			long free = myRuntime.freeMemory();
			maxFree = Math.max(maxFree, free);
			minFree = Math.min(minFree , free);
		}
	}
	
	public void reset() {
		synchronized(lock) {
			base = myRuntime.freeMemory();
			maxFree = base;
			minFree = base;
		}
	}

	public long getMinFree() {
		return minFree;
	}

	public long getMaxFree() {
		return maxFree;
	}
	
	public long getMinUsed() {
		return (base-maxFree);
	}
	
	public long getMaxUsed() {
		return (base-minFree);
	}

	public void cancel() {
		stopped = true;
	}
}
