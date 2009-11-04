package org.csiro.debug;

import javax.darjeeling.Darjeeling;

/**
 * A thin layer on top of Darjeeling.print to make sure no two threads go into
 * print at the same time. This is because we want to avoid race conditions in
 * the tinyOS split control stuff.
 * 
 * @author Niels Brouwers
 * 
 */
public class Debug {

	private static Object lock = new Object();
	private static boolean displayOutput = true;

	public static void setDisplayOutput(boolean on) {
		displayOutput = on;
	}

	public static void print(String str) {
		if (!displayOutput)
			return;

		synchronized (lock) {
			Darjeeling.print(str);
		}
	}

	public static void print(byte[] array) {
		if (!displayOutput)
			return;

		synchronized (lock) {
			Darjeeling.print(array);
		}
	}

	public static void print(int i) {
		if (!displayOutput)
			return;

		synchronized (lock) {
			Darjeeling.print(i);
		}
	}

}
