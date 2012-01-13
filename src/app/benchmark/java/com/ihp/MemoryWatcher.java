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
	private long maxFree = 0;
	private long minFree = -1;

	public MemoryWatcher() {
		myRuntime = Runtime.getRuntime();
	}
	
	public void run() {
		while (!stopped ) {
			try {
				Thread.sleep(10);
			} catch (Exception e) {
			}long free = myRuntime.freeMemory();
			maxFree = Math.max(maxFree, free);
			if (minFree < 0) {
				minFree = free;
			} else {
				minFree = Math.min(minFree , free);
			}
			try {
				Thread.sleep(40);
			} catch (Exception e) {
			}
		}
	}

	public long getMinFree() {
		return minFree;
	}

	public long getMaxFree() {
		return maxFree;
	}

	public void cancel() {
		stopped = true;
	}
}
