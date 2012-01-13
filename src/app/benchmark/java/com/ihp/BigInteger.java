/**
 * 
 */
package com.endosoft.java.math;

/**
 * @author Michael Maaser
 *
 */
public class BigInteger {

	private int[] value;
	
	public static final BigInteger ONE = new  BigInteger(new int[]{1});
	public static final BigInteger ZERO = new BigInteger(new int[]{0});
	
	private BigInteger(int[] ls) {
		value = trim(ls);
	}

	private int[] trim(int[] b) {
		if (b.length == 0) {
			return b;
		} else {
			int offSet = 0;
			int sign = b[0]>>31;
			//cuts off 0L s as int as the following bit is unset or if there is no more array element
			//otherwise
			//cuts off -1L s as int as there is a following bit and this is set 
			while (offSet < b.length && (b[offSet] == sign && followingBit(b, offSet) == sign)) {
				offSet++;
			}
			int[] result = new int[b.length - offSet];
			if (result.length > 0) {
				System.arraycopy(b, offSet, result, 0, result.length);
			}
			return result;
		}
	}

	private int followingBit(int[] b, int offSet) {
		if (offSet+1 >= b.length) {
			return 0;
		} else {
			return b[offSet+1]>>31;
		}
	}

	private byte followingBit(byte[] b, int offSet) {
		if (offSet+1 >= b.length) {
			return 0;
		} else {
			return (byte) (b[offSet+1]>>7);
		}
	}

	public static BigInteger valueOf(int val) {
		return new BigInteger(new int[] { val });
	}

	public BigInteger(byte[] val) {
		value = new int[(val.length + 7) >> 3];
		int j = (val.length % 8)-8;
		j = (j==-8)?0:j;
		int sign = val[0]>>7;
		for (int i = 0; i < value.length; i++) {
			value[i] = sign;
			for (int j2 = 0; j2 < 8; j2++) {
				value[i] <<= 8;
				if (j >= 0) {
					value[i] |= val[j] & 0xFF;
				} else {
					value[i] |= sign & 0xFF;
				}
				if (++j == val.length) {
					break;
				}
			}
		}
		value=trim(value);
	}

	public byte[] toByteArray() {
		int j = 0;
		byte[] result = new byte[(value.length << 2)];
		if (result.length == 0) {
			return new byte[] {0};
		}
		for (int i = 0; i < value.length; i++) {
			for (int j2 = 24; j2 >= 0; j2-=8) {
				result[j++] = (byte) ((value[i]>>>j2) & 0xFF);
			}
		}
		int offSet = 0;
		byte sign = (byte) (result[0]>>7);
		//cuts off 0L s as int as the following bit is unset or if there is no more array element
		//otherwise
		//cuts off -1L s as int as there is a following bit and this is set 
		while (offSet < result.length && (result[offSet] == sign && followingBit(result, offSet) == sign)) {
			offSet++;
		}
		if (offSet ==result.length) {
			offSet--;
		}
		byte[] trimmedResult = new byte[result.length - offSet];
		System.arraycopy(result, offSet, trimmedResult, 0, trimmedResult.length);
		return trimmedResult;
	}

	public int compareTo(BigInteger to) {
		if (signum() > to.signum()) {
			//greater by signum
			return 1;
		} else if (signum() < to.signum()) {
			return -1;
		} else if (value.length > to.value.length) {
//			System.out.println("greater by length");
			return 1;
		} else if (value.length < to.value.length) {
//			System.out.println("less by length");
			return -1;
		} else {
			for (int i = 0; i < value.length; i++) {
				if (value[i] != to.value[i]) {
					if ((value[i] >>> 32) == (to.value[i] >>> 32)) {
//						System.out.println("greater at "+ i + " this " + value[i] +" to "+ to.value[i]);
						return ((value[i] & 0xFFFFFFFFL) > (to.value[i]& 0xFFFFFFFFL))? 1:-1;
					} else {
//						System.out.println("less at "+ i + " this " + value[i] +" to "+ to.value[i]);
						return ((value[i] >>> 32) > (to.value[i] >>> 32))?1:-1;
					}
				}
			}
			return 0;
		}
//		int bitLength = this.bitLength();
//		if (bitLength > to.bitLength()) {
//			return 1;
//		} else if (bitLength < to.bitLength()) {
//			return -1;
//		} else {			
//			for (; bitLength >= 0; bitLength--) {
//				if (this.testBit(bitLength)) {
//					if (!to.testBit(bitLength)) {
//						return 1;
//					}
//				} else {
//					if (to.testBit(bitLength)) {
//						return -1;
//					}
//				}
//			}
//			return 0;
//		}
	}

	public boolean testBit(int i) {
		if (i >> 5 >= value.length) {
			return false;
		}
		return (value[value.length - 1 - (i >> 5)] & (1L << (i % 32))) != 0;
	}

	public BigInteger add(BigInteger summand2) {
		if (this.value.length == 0) {
			return summand2;
		} else if (summand2.value.length == 0) {
			return this;
		}
		int[] result = new int[Math.max(this.value.length, summand2.value.length) + 1];
		boolean carry = false;
		BigInteger summand1 = this;
		int resultPtr = result.length;
		int summand1Ptr = value.length;
		int summand2Ptr = summand2.value.length;
		int summand1Sign = summand1.value[0]>>31;
		int summand2Sign = summand2.value[0]>>31;
		while (resultPtr > 0) {
			resultPtr--;
			summand1Ptr--;
			summand2Ptr--;
			int summand1Lo = summand1Ptr<0?summand1Sign:summand1.value[summand1Ptr--];
			int summand2Lo = summand2Ptr<0?summand2Sign:summand2.value[summand2Ptr--];
			if ((summand1Lo | summand2Lo) == 0) {
				result[resultPtr--] = carry ? 1 : 0;
				carry = false;
			} else {
				int summand1Hi = summand1Ptr < 0 ? summand1Sign : summand1.value[summand1Ptr];
				int summand2Hi = summand2Ptr < 0 ? summand2Sign : summand2.value[summand2Ptr];

				boolean carryHandled = !carry;
				if (carry) {
					if ((summand1Lo & 0x01) == 0) {
						summand1Lo |= 0x01;
						carryHandled = true;
					} else if ((summand2Lo & 0x01) == 0) {
						summand2Lo |= 0x01;
						carryHandled = true;
					}
				}
				carry = add64Bit(summand1Hi, summand1Lo, summand2Hi,
						summand2Lo, result, resultPtr);
				if (!carryHandled) {
					result[resultPtr] |= 0x01L;
				}
				resultPtr--;
			}
		}
		return new BigInteger(result);
	}

	public BigInteger subtract(BigInteger subtrahend) {
		if (value.length == 0) {
			this.value = new int[] {0};
		}
		int valuePtr = value.length;
		int subtrahendPtr = subtrahend.value.length;
		if (subtrahendPtr == 0) {
			return this;
		}
		int[] result = new int[Math.max(valuePtr, subtrahendPtr)];
		int resultPtr = result.length;
		int minuend1Sign = this.value[0]>>31;
		int subtrahend2Sign = subtrahend.value[0]>>31;

		boolean carry = false;
		while (resultPtr > 0) {
			resultPtr--;
			valuePtr--;
			subtrahendPtr--;
			int minuendLo    = valuePtr<0     ? minuend1Sign    :           value[valuePtr--];
			int subtrahendLo = subtrahendPtr<0? subtrahend2Sign :subtrahend.value[subtrahendPtr--];
			boolean carryHandled = carry;
			if (!carry) {
				//diff = minuend - subtrahend;
				//diff = minuend + ~subtrahend + 1;  e.g. ~3 + 1 = 0xFC + 1 = -4 + 1 = -3
				if ((minuendLo & 0x01) == 0) {
					minuendLo |= 0x01; //diff = (minuend + 1) + ~subtrahend;
					carryHandled  = true;
				} else if ((subtrahendLo & 0x01) > 0) {
					subtrahendLo ^= 0x1; //diff = minuend + ~(subtrahend - 1);  e.g. ~(3-1) = ~2 = 0xFD = -3
					carryHandled = true;
				}
			}
			int minuendHi    = valuePtr < 0      ? minuend1Sign    :            value[valuePtr];
			int subtrahendHi = subtrahendPtr < 0 ? subtrahend2Sign : subtrahend.value[subtrahendPtr];
			carry = !add64Bit(minuendHi, minuendLo, ~subtrahendHi, ~subtrahendLo, result, resultPtr);
			if (!carryHandled) {
				result[resultPtr] |= 0x01; // diff = (minuend + ~subtrahend)
				// + 1; //the sum ends with 0 so
				// result + 1 == result | 1
			}
			resultPtr--;
		}
		return new BigInteger(result);
	}
	
	private static boolean isCarryOnAdd(int summand1, int summand2) {
		if (summand1 == 0 || summand2 == 0) {
			return false;
		} else {
			int MSBit1_2 = (summand1 >>> 31) + (summand2 >>> 31);
			int MSBit3 = ((summand1 & 0x7FFFFFFF) + (summand2 & 0x7FFFFFFF)) >>> 31;
			return (MSBit1_2 + MSBit3) > 1 ;
		}
	}

	public BigInteger mod(BigInteger modulus) {
		if (this.signum() < 0) {
			// add a multiple of modulus to get non negative value
			int shiftmod = 2 + this.bitLength() - modulus.bitLength();
			if (shiftmod <= 0) {
				return this.add(modulus);
			} else {
				return this.add(modulus.shiftLeft(shiftmod)).mod(modulus);
			}
		}
		if (this.value.length < modulus.value.length) {
			return this;
		}
		int bl2 = modulus.bitLength();
		if (this.bitLength() < bl2) {
			return this;
		}
		BigInteger result = this;

		while (modulus.compareTo(result) < 1) {
			int modShift = result.bitLength() - bl2 - 1;
			if (modShift > 0) {
				BigInteger shiftedModulus = modulus.shiftLeft(modShift);
				BigInteger remainder = result.subtract(shiftedModulus);
				result = remainder;
			} else {
				BigInteger remainder = result.subtract(modulus);
				result = remainder;
			}
		}
		return result;
	}

	private BigInteger shiftLeft() {
		if (value.length == 0) {
			return this;
		}
		int[] result;
		int idxR = 0, idxO = 0;
		if (value[0] >>> 31  > 0) {
			result = new int[value.length+1];
			result[0] = -1;
			idxR = 1;
		} else if (value[0] >>> 30 == 1) {
			result = new int[value.length+1];
			result[0] = 0;
			idxR = 1;
		} else {
			result = new int[value.length];
		}
		for (; idxR < result.length; idxR++) {
			result[idxR] = value[idxO] << 1;
			if (++idxO < value.length) {
				result[idxR] |= value[idxO] >>> 31;
			}
		}
		return new BigInteger(result);
	}

	private BigInteger shiftLeft(int bits) {
		if (value.length == 0) {
			return this;
		}
		int i32 = bits >> 5;
		BigInteger result = new BigInteger(shiftLeft32(value, i32));
		bits = bits & 31;
		if (bits != 0) {
			for (; bits > 0; bits--) {
				result = result.shiftLeft();
			}
		}
		return result;
	}
	
	private int[] shiftLeft32(int[] value, int i32) {
		if (value.length == 0) {
			return value;
		}
		int[] result = new int[value.length + i32];
		System.arraycopy(value, 0, result, 0, value.length);
		return result;
	}

	private BigInteger shiftRight(int i) {
		if (this.value.length == 0) {
			return this;
		}
		int i32 = i >> 5;
		int[] result=shiftRight32(value, i32);
		i = i & 31;
		if (i != 0) {
			int carryMask = 1 << i;
			carryMask--;
			int carryShift = 32 - i;
			int carry = 0;
			int carryNew = 0;
			carry = (result[0] & carryMask) << carryShift;
			result[0] >>= i; //keep signum
			for (int j = 1; j < result.length; j++) {
				carryNew = (result[j] & carryMask) << carryShift;
				result[j] >>>= i;
				result[j] |= carry;
				carry = carryNew;
			}
		}
		return new BigInteger(result);
	}

	private static int[] shiftRight32(int[] value, int i32) {
		if (value.length == 0) {
			return value;
		}
		int[] result = new int[value.length - i32 + 1];
		for (int i = 0; i < result.length-1; i++) {
			result[i+1] = value[i];
		}
		result[0] = value[0]>>31; //keep the signum
		return result;
	}

	private BigInteger shiftRight32(int i32) {
		return new BigInteger(shiftRight32(value, i32));
	}
	
//	private BigInteger shiftLeft32(int i32) {
//		return new BigInteger(shiftLeft32(value, i32));
//	}
	
	public int bitLength() {
		int result = value.length * 32;
		switch (signum()) {
		case 0:
			return 0;
		case 1:
			while (result > 0 && !testBit(result -1)) {
				result--;
			}
			return result;
		case -1:
			while (result > 0 && testBit(result -1)) {
				result--;
			}
			return result;
		}
		return 0;
	}

	public BigInteger negate() {
		return ZERO.subtract(this);
	}
	
	public int signum() {
		if (value.length == 0) {
			return 0;
		}
		else {
			return (int) (value[0]>>31 | 0x1);
		}
	}

	public BigInteger multiply(BigInteger factor2) {
		BigInteger factor1 = this;
		if (factor1.equals(ZERO)) {
			return ZERO;
		} else if (factor2.equals(ZERO)) {
			return ZERO;
		} else if (factor1.equals(ONE)) {
			return factor2;
		} else if (factor2.equals(ONE)) {
			return factor1;
		}
		boolean negateResult = false;
		if (factor1.signum() < 0) {
			factor1 = factor1.negate();
			negateResult = !negateResult;
		} 
		if (factor2.signum() < 0) {
			factor2 = factor2.negate();
			negateResult = !negateResult;
		} 
		int[] product = new int[factor1.value.length + factor2.value.length];
		int prodIdx = product.length;
		int f1Idx = factor1.value.length;
		f1Idx--;
		for (int i = 0; i < factor1.value.length; i++, f1Idx--) {
			prodIdx--;
			int f2Idx = factor2.value.length;
			f2Idx--;
			for (int j = 0; j < factor2.value.length; j++, f2Idx--) {
				int[] partialProduct = mult32Bit(factor1.value[f1Idx], factor2.value[f2Idx]);
				boolean carry = add64Bit(product[prodIdx-1], product[prodIdx], partialProduct[0], partialProduct[1], product, prodIdx);
				prodIdx--;
				if (carry) {
					product[prodIdx-1]++;
				}
			}
			prodIdx += factor2.value.length;
		}
		if (negateResult) {
			return new BigInteger(product).negate();			
		} else {
			return new BigInteger(product);
		}
	}
	
	private BigInteger multiply(int factor2) {
		BigInteger factor1 = this;
		if (factor1.equals(ZERO)) {
			return ZERO;
		} else if (factor2 == 0) {
			return ZERO;
		} else if (factor1.equals(ONE)) {
			return BigInteger.valueOf(factor2);
		} else if (factor2==1) {
			return factor1;
		}
		int[] product = new int[factor1.value.length + 1];
		int prodIdx = product.length;
		int f1Idx = factor1.value.length;
		f1Idx--;
		for (int i = 0; i < factor1.value.length; i++, f1Idx--) {
			prodIdx--;
			int f2Idx = 1;
			f2Idx--;
				int[] partialProduct = mult32Bit(factor1.value[f1Idx], factor2);
				boolean carry = add64Bit(product[prodIdx-1], product[prodIdx], partialProduct[0], partialProduct[1], product, prodIdx);
				prodIdx--;
				if (carry) {
					product[prodIdx-1]++;
				}
			prodIdx += 1;
		}
		return new BigInteger(product);
	}
	
	
	public BigInteger modMult(BigInteger factor2, BigInteger modulus) {
		BigInteger factor1 = this.transferToMontgomeryDomain(modulus);	
		factor2 = factor2.transferToMontgomeryDomain(modulus);
		int m_ = montgomeryModInv(modulus);
		BigInteger montyProduct = factor1.modMultMontgomery(factor2, modulus, m_);
		BigInteger reduced = montyProduct.montgomeryReductionAnalogSHuss(modulus, m_);
		return reduced;
	}
	
	private BigInteger transferToMontgomeryDomain(BigInteger modulus) {
		int bitsOfR = modulus.value.length<<5;
		BigInteger result = this;
		int numberOfBitsLessThanModulus = modulus.bitLength()-result.bitLength();
		if (numberOfBitsLessThanModulus <= 0) {
			result = result.mod(modulus);
		} else {
			result = result.shiftLeft(numberOfBitsLessThanModulus).mod(modulus);
			bitsOfR -=  numberOfBitsLessThanModulus;
		}
		
		for (int l = bitsOfR;l > 0;l--) {
			result = result.shiftLeft();
			if (result.compareTo(modulus) > 0) {
				result = result.subtract(modulus);
			}
		}
		return result;
	}

	/**
	 * @param montyProduct
	 * @param modulus
	 * @return
	 */
	private BigInteger montgomeryReductionAnalogSHuss(BigInteger modulus, int m_) {
		if (value.length == 0) {
			return this;
		}
		BigInteger result = this;
		for (int i = 0; i < modulus.value.length; i++) {
			int r_0 = result.value[result.value.length-1];
			int[] t = mult32Bit(r_0, m_);
			result = result.add(modulus.multiply(t[1])).shiftRight32(1);
		}
		if (result.compareTo(modulus) >= 0) {
			result = result.subtract(modulus);
		}
		return result;
	}
	
	private static int modInv(int a) {
		int result = 0;
		int r = 1;
		for (int b = 32; b > 0; b--) {
			// a*_m + r = 0 mod 2^b
			if ((r & 0x01) == 0) {
				r >>= 1;
			} else {
				result |= (1L << (32 - b));
				r += a;
				r >>= 1;
			}
		}
		return result;
	}
		
	private BigInteger modMultMontgomery(BigInteger factor2,
			BigInteger modulus, int montgomeryModInv) {
		BigInteger factor1 = this;
		BigInteger result = factor1.modMultMontgomeryNachSHuss(factor2, modulus, montgomeryModInv);
		return result;
	}

	/**
	 * @param modulus
	 * @return
	 */
	private static int montgomeryModInv(BigInteger modulus) {
		return modInv(modulus.value[modulus.value.length -1]);
	}

	private BigInteger modMultMontgomeryNachSHuss(BigInteger factor2, BigInteger modulus, int m_) {
		BigInteger factor1 = this;
		if (this.value.length == 0) {
			return ZERO;
		} else {
			factor1 = this;//.mod(modulus);
			if (factor1.value.length == 0) {
				return ZERO;
			}
		}
		if (factor2.value.length == 0) {
			return ZERO;
//		} else {
//			factor2 = factor2.mod(modulus);
//			if (factor2.value.length == 0) {
//				return ZERO;
//			}
		}
		BigInteger result = ZERO;
		int y_0 = factor2.value[factor2.value.length-1];
		for (int i = 0; i < modulus.value.length; i++) {
			int x_i = i < factor1.value.length?factor1.value[factor1.value.length-1-i]:0; // I can use 0 for unset longs, i.e., indexes < 0 because I made sure that the factors are positive less than modulus
			int[] u = mult32Bit(x_i, y_0);
			int r_0 = result.value.length==0?0:result.value[result.value.length-1];
			add64Bit(0, r_0 ,u[0], u[1], u, 1);
			u = mult32Bit(u[1], m_);
			
			result = result.add(factor2.multiply(x_i)).add(modulus.multiply(u[1])).shiftRight32(1);
		}
		//TODO possibly replace with mod();
		if (result.compareTo(modulus) >= 0) {
			result = result.subtract(modulus);
		}
		return result;
	}
	
	public BigInteger modPow(BigInteger exp, BigInteger modulus) {
		return modPowMontgomeryUseInterleavedExponentiation(exp, modulus);
	}

	private BigInteger modPowMontgomeryUseInterleavedExponentiation(BigInteger exp, BigInteger modulus) {
		if (exp.signum() < 0) {
			throw new ArithmeticException("negative exponent not supported");
		}
		if (modulus.signum() < 1) {
			throw new ArithmeticException("modulus must be positive");
		} else if (modulus.equals(ONE)) {
			return ZERO;
		}
		if (exp.value.length == 0) {
			return ONE;
		} else {
			BigInteger result = ONE;
			BigInteger base = null;
			int bitLength = exp.bitLength();
			int m_ = montgomeryModInv(modulus);// (-modulus^-1) mod 2^64
			for (int i = 0 ; i < bitLength; i++) {
				if (base == null) {
//					System.out.println("xfer base into monty domain");
					base = this.transferToMontgomeryDomain(modulus);
//					System.out.println("base xfer'd");
				} else {
					base = base.modMultMontgomery(base, modulus, m_);//.montgomeryReductionAnalogSHuss(modulus);
				}
				if (exp.testBit(i)) {
					result = result.modMultMontgomery(base, modulus, m_);//.montgomeryReductionAnalogSHuss(modulus);
				}
			}
			return result;
		}
	}
	
	private static boolean add64Bit(int summand1Hi, int summand1Lo,
			int summand2Hi, int summand2Lo, int[] result, int resultOffset) {
		result[resultOffset--] = summand1Lo + summand2Lo;
		boolean carry = isCarryOnAdd(summand1Lo, summand2Lo);
		int help = summand1Hi + summand2Hi;
		if (resultOffset >= 0) {
			result[resultOffset--] = help + (carry ? 1 : 0);
			if (isCarryOnAdd(summand1Hi, summand2Hi)) {
				return true;
			} else {
				return (help == -1) && carry; // if help == -1 ==
												// 0xFFFFFFFFFFFFFFFF then
												// addition of will overflow to
												// "carry flag set" and 0x0000;
			}
		} else {
			if (help == (carry ? 0 : -1)) {
				return carry;
			} else {
				return true;
			}
		}
	}

	/**
	 * 
	 * @param factor1
	 * @param factor2
	 * @return product as 128bits [0] are the MSBits [1] are the LSBits 
	 */
	private static int[] mult32Bit(int factor1, int factor2) {
		int[] result = new int[2];
		int a = factor1 >>> 16;
		int b = factor1 & 0xFFFF;
		int c = factor2 >>> 16;
		int d = factor2 & 0xFFFF;
		result[0] = a * c;
		result[1] = b * d;
		int m1 = a * d;
		result[0] += m1 >>> 16;
		int m2 = b * c;
		result[0] += m2 >>> 16;
		int m3 = (m1 << 16);
		if (isCarryOnAdd(result[1], m3)) {
			result[0]++;
		}
		result[1] += m3;
		int m4 = (m2 << 16);
		if (isCarryOnAdd(result[1], m4)) {
			result[0]++;
		}
		result[1] += m4;
		return result;
	}
	
	public boolean equals(BigInteger x) {
		if (x == null) {
			return false;
		} else {
			return compareTo(x) == 0;
		}
	}
	
	public String toString() {
		byte[] ba = toByteArray();
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < ba.length; i++) {
			if ((ba[i] & 0xFF) < 16) {
				sb.append('0');
			}
			sb.append(Integer.toHexString(ba[i] & 0xFF));
			sb.append((ba.length - i)%4 ==1?'.':' ');
		}
		return sb.toString();
	}
	
	public BigInteger modInv(BigInteger modulus) {
		BigInteger[] result = new BigInteger[] {ZERO, this, ZERO, modulus, ONE};
		solveEuklid(result);
		return result[0].mod(modulus);
	}

	private void solveEuklid(BigInteger[] params) {
//		System.out.println("X * " + params[1] + " = k * " + params[3] + " + " + params[4]);
		if (params[1].equals(ONE)) {
			params[2] = ZERO;
			params[0] = params[4];
//			System.out.println(params[0] + " * " + params[1] + " = " + params[2] +" * " + params[3] + " + " + params[4]);
//			System.out.println(params[0].multiply(params[1]) + " = " + params[2].multiply(params[3]).add(params[4]));
		} else {
			BigInteger[] params_ = new BigInteger[] {ZERO,params[3].mod(params[1]), ZERO, params[1],params[4].negate()};
			solveEuklid(params_);
			params[2] = params_[0];
			params[0] = params_[2].add(params[3].divideWithoutRemainder(params[1]).multiply(params[2]));
//			System.out.println(params[0] + " * " + params[1] + " = " + params[2] +" * " + params[3] + " + " + params[4]);			
//			System.out.println(params[0].multiply(params[1]) + " = " + params[2].multiply(params[3]).add(params[4]));
		}
	}

	public BigInteger divideWithoutRemainder(BigInteger divisor) {
		BigInteger result = ZERO;
		BigInteger dividend = this;
		boolean negateResult = false;
		if (dividend.signum() < 0) {
			dividend = dividend.negate();
			negateResult = !negateResult;
		} 
		if (divisor.signum() < 0) {
			divisor = divisor.negate();
			negateResult = !negateResult;
		} 
		while (dividend.compareTo(divisor) >= 0) {
			BigInteger shiftedDivisor = divisor;
			int shiftedBits = 0;
			while (dividend.compareTo(shiftedDivisor = shiftedDivisor.shiftLeft()) >= 0) {
				shiftedBits++;
			}
			shiftedDivisor = shiftedDivisor.shiftRight(1);
			result = result.add(ONE.shiftLeft(shiftedBits));
			dividend = dividend.subtract(shiftedDivisor);
		}
		if (negateResult) {
			return result.negate();
		} else {
			return result;
		}
	}
}
