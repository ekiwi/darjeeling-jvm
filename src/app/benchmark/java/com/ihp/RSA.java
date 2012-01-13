/**
 * 
 */
package com.ihp;

import com.endosoft.java.math.BigInteger;

/**
 * @author Michael Maaser
 *
 */
public class RSA implements TestImplementation {

	private BigInteger msg;
	private BigInteger pubExp;
	private BigInteger privExp;
	private BigInteger modulus;

	public RSA() {
		BigInteger p = BigInteger.valueOf(909091);
		System.out.println("P=" + p);
		BigInteger q = BigInteger.valueOf(858577);
		System.out.println("Q=" + q);

		 modulus = p.multiply(q);
		System.out.println("modulus=" + modulus);
		BigInteger phiOfModulus = p.subtract(BigInteger.ONE).multiply(
				q.subtract(BigInteger.ONE));
		System.out.println("phi(N)=" + phiOfModulus);
		msg = BigInteger.valueOf(176506078);
		System.out.println("msg=" + msg);
		 pubExp = BigInteger.valueOf(257);
		 privExp = pubExp.modInv(phiOfModulus);
		System.out.println("pub = " + pubExp);
		System.out.println("priv= " + privExp);
		BigInteger prod = pubExp.multiply(privExp);
		System.out.println("prod=" + prod);
		BigInteger prodModPhi = prod.mod(phiOfModulus);
		System.out.println("prod % phi=" + prodModPhi);
	}
	
	/* (non-Javadoc)
	 * @see com.ihp.TestImplementation#runTest(int)
	 */
	public void runTest(int times) {
		for (;times > 0; times--) {
			BigInteger result = msg.modPow(pubExp, modulus);
//			System.out.println("enc msg="+result);
			BigInteger result2 = result.modPow(privExp, modulus);
//			System.out.println("dec msg="+result2);
		}
	}

	/* (non-Javadoc)
	 * @see com.ihp.TestImplementation#getName()
	 */
	public String getName() {
		return "RSA";
	}

}
