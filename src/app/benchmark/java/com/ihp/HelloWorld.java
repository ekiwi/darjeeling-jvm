package com.ihp;

/**
 * A typical HelloWorld Program, framed by a measurement bracket.
 * @author Oliver Maye
 * @version 1.0
 */
public class HelloWorld implements TestImplementation {

	public void runTest(int times) {
        System.out.println("Hello World!");
    }

	public String getName() {
		return "HelloWorld";
	}
	
}
