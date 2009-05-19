import javax.darjeeling.Darjeeling;

public class ArithmeticTest
{
	
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
		
		/*
		byte a,b,c;
		boolean d;
		int e = 1;
		
		a = 64;
		b = 2;

		e = a + b + b;
		
		// exercise standard arithmetic
		Darjeeling.assertTrue(testBase+ 0, d=((c=(byte)(a+b))==66));
		
		*/
		
//		boolean pass;
//        byte    barr[];
//        short   sarr[];
//        int     iarr[];
//        Object  oarr[];
//
//        
//		// test byte array new
//		barr = new byte[100];
//		Darjeeling.assertTrue(testBase+00, barr!=null);
//		Darjeeling.assertTrue(testBase+01, barr.length == 100);
//
//		// test byte array store/load
//		pass = true;
//		for (int i=0; i<barr.length; i++)
//			barr[i] = (byte)i;
//		for (int i=0; i<barr.length; i++)
//			pass=pass && (barr[i]==i);
//		Darjeeling.assertTrue(testBase+02, pass);
//
//		// test byte array initialisation
//		barr = new byte[] { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
//		Darjeeling.assertTrue(testBase+03, barr!=null);
//		pass = true;
//		for (int i=0; i<barr.length; i++)
//			pass &= (barr[i]==i-5);
//		Darjeeling.assertTrue(testBase+04, pass);
//
//        // test default initialisation
//        barr = new byte[5];
//        Darjeeling.assertTrue(testBase+5, barr[3]==0);
//        sarr = new short[5];
//        Darjeeling.assertTrue(testBase+6, sarr[3]==0);
//        iarr = new int[5];
//        Darjeeling.assertTrue(testBase+7, iarr[3]==0);
//        oarr = new Object[5];
//        Darjeeling.assertTrue(testBase+8, oarr[3]==null);
//
//		// test short array new
//		sarr = new short[100];
//		Darjeeling.assertTrue(testBase+10, sarr!=null);
//		Darjeeling.assertTrue(testBase+11, sarr.length == 100);
//		
//		// test short array store/load
//		pass = true;
//		for (int i=0; i<sarr.length; i++) sarr[i] = (short)i;
//		for (int i=0; i<sarr.length; i++) pass=pass && (sarr[i]==i);
//		Darjeeling.assertTrue(testBase+12, pass);
//		
//		// test integer array new
//		iarr = new int[100];
//		Darjeeling.assertTrue(testBase+20, iarr!=null);
//		Darjeeling.assertTrue(testBase+21, iarr.length == 100);
//		
//		// test integer array store/load
//		pass = true;
//		for (int i=0; i<iarr.length; i++)
//			iarr[i] = i;
//		for (int i=0; i<iarr.length; i++)
//			pass=pass && (iarr[i]==i);
//		Darjeeling.assertTrue(testBase+22, pass);
//		
//		// there was a bug in IASTORE that wasn't caught by the above tests,
//		// this this is here to keep that bug from coming back :)
//		int intTest[] = new int[1];
//		intTest[0] = 0x67452301;
//		Darjeeling.assertTrue(testBase+23, intTest[0] == 0x67452301);
//		
//		// char arrays weren't tested before :)
//		char carr[] = new char[] { 'a', 'b', 'c', 'd' };
//		Darjeeling.assertTrue(testBase+24, carr[0] == 'a');
//		Darjeeling.assertTrue(testBase+25, carr[1] == 'b');
//		Darjeeling.assertTrue(testBase+26, carr[2] == 'c');
//		Darjeeling.assertTrue(testBase+27, carr[3] == 'd');
		
	}

	public static void main(String[] args)
	{
		testByte(0);
	}

}
