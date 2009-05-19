package testvm.tests;

import javax.darjeeling.Darjeeling;

public class ArithmeticTest
{
	
	private static int rotate_left(int x, int n)
	{
		return (x << n) | (x >>> (32 - n));
	}
	
	private static void testByte(int testBase)
	{
		byte a,b,c;
		
		
		a = 64;
		b = 2;
		
		// exercise standard arithmetic
		Darjeeling.assertTrue(testBase+ 0, (c=(byte)(a+b))==66);
		Darjeeling.assertTrue(testBase+ 1, a-b==62);
		Darjeeling.assertTrue(testBase+ 2, a/b==32);
		Darjeeling.assertTrue(testBase+ 3, a*b==128);

		Darjeeling.assertTrue(testBase+ 4, -a==-64);
		Darjeeling.assertTrue(testBase+ 5, a%b==0);

		// shr
		Darjeeling.assertTrue(testBase+ 6, a>>1==32);
		Darjeeling.assertTrue(testBase+ 7, a>>3==8);
		Darjeeling.assertTrue(testBase+ 8, a>>5==b);

		// shl
		Darjeeling.assertTrue(testBase+ 9, a<<1==128);
		Darjeeling.assertTrue(testBase+10, b<<3==16);
		Darjeeling.assertTrue(testBase+11, b<<7==256);
		
		// iinc
		a++;
		Darjeeling.assertTrue(testBase+12, a==65);
		a--;
		Darjeeling.assertTrue(testBase+13, a==64);
		a+=64;
		Darjeeling.assertTrue(testBase+14, a==-128);
		a-=64;

		// exercise boolean logic
		Darjeeling.assertTrue(testBase+15, (a & b)==0);
		Darjeeling.assertTrue(testBase+16, (a | b)==66);
		Darjeeling.assertTrue(testBase+17, (a ^ b)==66);
		
		// overflow tests
		a = 0x7f;
		b = 1;
		c = 2;
		Darjeeling.assertTrue(testBase + 18, a + b == 128);
		
		Darjeeling.assertTrue(testBase + 19, (byte)(a+b) == -128);
		Darjeeling.assertTrue(testBase + 20, (byte)(a + b + b) == -127);
		
		Darjeeling.assertTrue(testBase + 21, (a + b) / c == 64);
		Darjeeling.assertTrue(testBase + 22, (byte)((a+b) / c) == 64);
	}

	private static void testShort(int testBase)
	{
		short a,b,c;
		
		a = 64;
		b = 2;
		
		// exercise standard arithmetic
		Darjeeling.assertTrue(testBase+ 0, a+b==66);
		Darjeeling.assertTrue(testBase+ 1, a-b==62);
		Darjeeling.assertTrue(testBase+ 2, a/b==32);
		Darjeeling.assertTrue(testBase+ 3, a*b==128);

		Darjeeling.assertTrue(testBase+ 4, -a==-64);
		Darjeeling.assertTrue(testBase+ 5, a%b==0);

		// shr
		Darjeeling.assertTrue(testBase+ 6, a>>1==32);
		Darjeeling.assertTrue(testBase+ 7, a>>3==8);
		Darjeeling.assertTrue(testBase+ 8, a>>5==b);

		// shl
		Darjeeling.assertTrue(testBase+ 9, a<<1==128);
		Darjeeling.assertTrue(testBase+10, b<<3==16);
		Darjeeling.assertTrue(testBase+11, b<<7==256);
		
		// iinc
		a++;
		Darjeeling.assertTrue(testBase+12, a==65);
		a--;
		Darjeeling.assertTrue(testBase+13, a==64);
		a+=1000;
		Darjeeling.assertTrue(testBase+14, a==1064);
		a-=1000;

		// exercise boolean logic
		Darjeeling.assertTrue(testBase+15, (a & b)==0);
		Darjeeling.assertTrue(testBase+16, (a | b)==66);
		Darjeeling.assertTrue(testBase+17, (a ^ b)==66);
		
		// overflow tests
		a = 0x00007fff;
		b = 1;
		c = 2;
		Darjeeling.assertTrue(testBase + 18, a + b == 32768);
		
		Darjeeling.assertTrue(testBase + 19, (short)(a+b) == -32768);
		Darjeeling.assertTrue(testBase + 20, (short)(a + b + b) == -32767);
		
		Darjeeling.assertTrue(testBase + 21, (a + b) / c == 16384);
		Darjeeling.assertTrue(testBase + 22, (short)((a+b) / c) == 16384);
		
	}
	
	private static void testInt(int testBase)
	{
		int a,b;
		
		a = 64;
		b = 2;
		
		// exercise standard arithmetic
		Darjeeling.assertTrue(testBase+ 0, a+b==66);
		Darjeeling.assertTrue(testBase+ 1, a-b==62);
		Darjeeling.assertTrue(testBase+ 2, a/b==32);
		Darjeeling.assertTrue(testBase+ 3, a*b==128);

		Darjeeling.assertTrue(testBase+ 4, -a==-64);
		Darjeeling.assertTrue(testBase+ 5, a%b==0);

		// shr
		Darjeeling.assertTrue(testBase+ 6, a>>1==32);
		Darjeeling.assertTrue(testBase+ 7, a>>3==8);
		Darjeeling.assertTrue(testBase+ 8, a>>5==b);

		// shl
		Darjeeling.assertTrue(testBase+ 9, a<<1==128);
		Darjeeling.assertTrue(testBase+10, b<<3==16);
		Darjeeling.assertTrue(testBase+11, b<<7==256);
		
		// iinc
		a++;
		Darjeeling.assertTrue(testBase+12, a==65);
		a--;
		Darjeeling.assertTrue(testBase+13, a==64);
		a+=1000;
		Darjeeling.assertTrue(testBase+14, a==1064);
		a-=1000;

		// exercise boolean logic
		Darjeeling.assertTrue(testBase+15, (a & b)==0);
		Darjeeling.assertTrue(testBase+16, (a | b)==66);
		Darjeeling.assertTrue(testBase+17, (a ^ b)==66);
		
		// exercise LDC
		a = 123456;
		Darjeeling.assertTrue(testBase+18, (a >>> 16) == 1);
		Darjeeling.assertTrue(testBase+19, (a >>> 8) == 482);
		Darjeeling.assertTrue(testBase+20, (-a >>> 8) == 16776733);

		a = 0x0000ffff;
		Darjeeling.assertTrue(testBase+21, (a >>> 8) == 255);
		a = 0x00ffffff;
		Darjeeling.assertTrue(testBase+22, (a >>> 16) == 255);
		a = 0xffffffff;
		Darjeeling.assertTrue(testBase+23, (a >>> 24) == 255);
		
		Darjeeling.assertTrue(testBase+24, rotate_left(1, 1) == 2);   
		Darjeeling.assertTrue(testBase+25, rotate_left(-1, 1) == -1);   
		Darjeeling.assertTrue(testBase+26, rotate_left(-2, 1) == -3);
		Darjeeling.assertTrue(testBase+27, rotate_left(-2, 16) == -65537);   

		Darjeeling.assertTrue(testBase+28, 0 - 1 == -1);
		
		// this loop will stall if IINC isn't handled properly
		for (int i=0; i<33000; i++)
		{
		}
		Darjeeling.assertTrue(testBase + 29, true);
		
	}

	public static void test(int testBase)
	{
		testByte(testBase);
		testShort(testBase+30);
		testInt(testBase+60);
	}

}
