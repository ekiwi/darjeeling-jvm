/**
 * 
 */
package com.ihp;


/**
 * @author Michael Maaser
 * 
 */
public class Benchmark {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		test(new HelloWorld());
		test(new BubbleSort());
		test(new QuickSort());
		test(new TowersOfHanoi((short)12));
		test(new RSA());
		
		
	}
	
	private static void test(TestImplementation timpl) {
		MemoryWatcher memoryWatcher = startMemoryWatcher();
		for (int i = 0; i < 6; i++) {
			int j = 1 << i;
			memoryWatcher.explicitGC();
			memoryWatcher.reset();

			long start = System.currentTimeMillis();
			timpl.runTest(j);
			long end = System.currentTimeMillis();
			memoryWatcher.measure();
			memoryWatcher.explicitGC();
			memoryWatcher.measure();
			long min = memoryWatcher.getMinUsed();
			long max = memoryWatcher.getMaxUsed();

			System.out.println(timpl.getName() + "(" + j + "x)" + " took " +
								(end - start) + "ms. Memory use: " +
								min + " - " + max );
		}
		memoryWatcher.cancel();
	}
	
	private static MemoryWatcher startMemoryWatcher() {
		MemoryWatcher mw = new MemoryWatcher();
		mw.start();
		return mw;
	}

}
