/**
 * 
 */
package com.ihp;

import javax.darjeeling.Darjeeling;

/**
 * @author Michael Maaser
 * 
 */
public class Benchmark {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Here we go!");
		Runtime r = Runtime.getRuntime();
		printMemory(r);
		TestImplementation ti = new Sort();
		System.out.println( ti.toString() );
		System.out.println( ti.getName() );
		
		ti.runTest(16);
		printMemory(r);
		
//		try {
//		test(new Sort());
//		} catch (Exception e) {
//			
//		}
//		test(new BubbleSort());
//		test(new QuickSort());
//		test(new TowersOfHanoi((short)12));
//
//		test(new RSA());
	}
	
//	public static void synchronousGC() {
//		Runtime.getRuntime().gc();
//		long free1 = 0;
//		long free2 = 0;
//		do {
//			free1 = free2;
//			free2 = Runtime.getRuntime().freeMemory();
//			try {
//				Thread.sleep(1000);
//			} catch (Exception e) {
//			}
//		} while (free2 != free1);
//	}

	private static void printMemory(Runtime r) {
		long free = r.freeMemory();
		String freeS = Long.toString(free);
		Darjeeling.print(freeS);
		Darjeeling.print(" - ");
		long total = r.totalMemory();
		String totalS = Long.toString(total);
		Darjeeling.print(totalS);
		Darjeeling.print("\n");
	}

	/*private static void test(TestImplementation timpl) {
		Darjeeling.println("test()");
		Runtime runtime = Runtime.getRuntime();
		for (int i = 0; i < 6; i++) {
			int j = 1 << i;
			Darjeeling.println("freeMem()=");
			long freebefore = runtime.freeMemory();
			Darjeeling.println(Long.toString(freebefore));
//			MemoryWatcher memoryWatcher = startMemoryWatcher();
			long start = System.currentTimeMillis();
			Darjeeling.println(Integer.toString(j));
			timpl.runTest(j);
			Darjeeling.println("tests run.");
//			memoryWatcher.cancel();
			long end = System.currentTimeMillis();
			Darjeeling.println("gc()");
//			long freeafter = Runtime.getRuntime().freeMemory();
//			long freeDuringMin = 0;//memoryWatcher.getMinFree();
//			long freeDuringMax = memoryWatcher.getMaxFree();
			runtime.gc();
			Darjeeling.println(timpl.getName());
//			Darjeeling.println(Long.toString(freebefore));
			long duration = end - start;
			Darjeeling.println(Long.toString(duration));
//			Darjeeling.println(timpl.getName() + "(" + j + " x)" + " took "
//					+ (end - start) + "ms");
//			Darjeeling.println("memory use:"+/*(freebefore-freeDuringMax)+" - "* /+(freebefore-freeDuringMin));
			
		}
	}*/
	
//	private static MemoryWatcher startMemoryWatcher() {
//		MemoryWatcher mw = new MemoryWatcher();
//		mw.start();
//		return mw;
//	}

//	private static void dump(short[] testfield) {
//		for(int i = 0; i < testfield.length;) {
//			String dumpline = "";
//			for (int j = 0; j < 16 && i < testfield.length; j++, i++) {
//				dumpline += testfield[i] + " ";
//			}
//			Darjeeling.println(dumpline);
//		}
//	}
}
