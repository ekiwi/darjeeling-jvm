/**
 * 
 */
package com.ihp;

import javax.darjeeling.Darjeeling;

/**
 * @author Michael Maaser
 *
 */
public class Sort implements TestImplementation {

	private short[] testfield;

	private short modpow(int i, int j, int length) {
		short result = 1;
		for (;j>0;j--) {
			result = (short) ((result * i) % length);
		}
		return result;
	}

	/* (non-Javadoc)
	 * @see com.ihp.TestImplementation#runTest(int)
	 */
	public void runTest(int times) {
		testfield = new short[(short) 253];
		Darjeeling.print("run\n");
		Runtime r = Runtime.getRuntime();
		for (; times > 0; times--) {
			Darjeeling.print(Integer.toString(times));
//			long free = r.freeMemory();
			Darjeeling.print(" - ");
//			String freeS = Long.toString(free);
//			Darjeeling.print(freeS);
			Darjeeling.print("\n");
			prepareSortTestField();
//			Benchmark.synchronousGC();
		}
	}

	protected void prepareSortTestField() {
		for (int i = 0; i < testfield.length; i++) {
			testfield[i] = modpow(i, 17, testfield.length);
		}
	}

	/* (non-Javadoc)
	 * @see com.ihp.TestImplementation#getName()
	 */
	public String getName() {
		return "SortPreparation";
	}

}
