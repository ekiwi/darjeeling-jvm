/**
 * 
 */
package com.ihp;


/**
 * @author Michael Maaser
 *
 */
public class Sort implements TestImplementation {

	protected short[] testfield = new short[(short) 253];

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
		for (; times > 0; times--) {
			prepareSortTestField();
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
