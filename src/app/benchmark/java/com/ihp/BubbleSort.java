/**
 * 
 */
package com.ihp;

/**
 * @author Michael Maaser
 *
 */
public class BubbleSort extends Sort implements TestImplementation {

	/* (non-Javadoc)
	 * @see com.ihp.TestImplementation#runTest()
	 */
	public void runTest(int times) {
		for (; times > 0; times--) {
			short[] testfield = prepareSortTestField((short) 253);
//			 dump(testfield);
			bubbleSort(testfield);
//			 dump(testfield);
		}
	}

	/* (non-Javadoc)
	 * @see com.ihp.TestImplementation#getName()
	 */
	public String getName() {
		return "BubbleSort";
	}

	private void bubbleSort(short[] testfield) {
	
		for (int i = 0; i < testfield.length; i++) {
			for (int j = 0; j < testfield.length - i - 1; j++)
				if (testfield[j] > testfield[j + 1]) {
					short temp = testfield[j];
					testfield[j] = testfield[j + 1];
					testfield[j + 1] = temp;
				}
		}
	}

}
