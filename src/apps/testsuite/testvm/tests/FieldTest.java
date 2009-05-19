package testvm.tests;

import javax.darjeeling.Darjeeling;

import testvm.classes.B;

public class FieldTest
{
	
	private static int _integer;
	private static byte _byte;
	private static short _short;
	private static boolean _boolean;
	private static int[] _inta;

	private static void testInteger(int testBase)
	{
		B b = new B();
		_integer = 10;
		Darjeeling.assertTrue(testBase +  0, _integer == 10);
		b.integer1 = _integer;
		Darjeeling.assertTrue(testBase +  1, _integer == b.integer1);
		Darjeeling.assertTrue(testBase +  2, b.integer1 == 10);

		_integer = 20;
		Darjeeling.assertTrue(testBase +  3, _integer == 20);
		b.integer2 = _integer;
		Darjeeling.assertTrue(testBase +  4, _integer == b.integer2);
		Darjeeling.assertTrue(testBase +  5, b.integer2 == 20);

		_integer = 30;
		Darjeeling.assertTrue(testBase +  6, _integer == 30);
		b.integer3 = _integer;
		Darjeeling.assertTrue(testBase +  7, _integer == b.integer3);
		Darjeeling.assertTrue(testBase +  8, b.integer3 == 30);

	}
	
	private static void testShort(int testBase)
	{
		B b = new B();
		_short = 10;
		Darjeeling.assertTrue(testBase +  0, _short == 10);
		b.short1 = _short;
		Darjeeling.assertTrue(testBase +  1, _short == b.short1);
		Darjeeling.assertTrue(testBase +  2, b.short1 == 10);

		_short = 20;
		Darjeeling.assertTrue(testBase +  3, _short == 20);
		b.short2 = _short;
		Darjeeling.assertTrue(testBase +  4, _short == b.short2);
		Darjeeling.assertTrue(testBase +  5, b.short2 == 20);

		_short = 30;
		Darjeeling.assertTrue(testBase +  6, _short == 30);
		b.short3 = _short;
		Darjeeling.assertTrue(testBase +  7, _short == b.short3);
		Darjeeling.assertTrue(testBase +  8, b.short3 == 30);

	}
	
	private static void testByte(int testBase)
	{
		B b = new B();
		_byte = 10;
		Darjeeling.assertTrue(testBase +  0, _byte == 10);
		b.byte1 = _byte;
		Darjeeling.assertTrue(testBase +  1, _byte == b.byte1);
		Darjeeling.assertTrue(testBase +  2, b.byte1 == 10);

		_byte = 20;
		Darjeeling.assertTrue(testBase +  3, _byte == 20);
		b.byte2 = _byte;
		Darjeeling.assertTrue(testBase +  4, _byte == b.byte2);
		Darjeeling.assertTrue(testBase +  5, b.byte2 == 20);

		_byte = 30;
		Darjeeling.assertTrue(testBase +  6, _byte == 30);
		b.byte3 = _byte;
		Darjeeling.assertTrue(testBase +  7, _byte == b.byte3);
		Darjeeling.assertTrue(testBase +  8, b.byte3 == 30);

	}	
	
	private static void testBoolean(int testBase)
	{
		B b = new B();
		_boolean = true;
		Darjeeling.assertTrue(testBase +  0, _boolean == true);
		b.boolean1 = _boolean;
		Darjeeling.assertTrue(testBase +  1, _boolean == b.boolean1);
		Darjeeling.assertTrue(testBase +  2, b.boolean1 == true);

		_boolean = false;
		Darjeeling.assertTrue(testBase +  3, _boolean == false);
		b.boolean2 = _boolean;
		Darjeeling.assertTrue(testBase +  4, _boolean == b.boolean2);
		Darjeeling.assertTrue(testBase +  5, b.boolean2 == false);

		_boolean = true;
		Darjeeling.assertTrue(testBase +  6, _boolean == true);
		b.boolean3 = _boolean;
		Darjeeling.assertTrue(testBase +  7, _boolean == b.boolean3);
		Darjeeling.assertTrue(testBase +  8, b.boolean3 == true);

	}	
	
	private static void testIntArray(int testBase)
	{
		B b = new B();
		int[] inta = new int[10];
		_inta = inta;
		Darjeeling.assertTrue(testBase +  0, _inta == inta);
		b.inta1 = _inta;
		Darjeeling.assertTrue(testBase +  1, _inta == b.inta1);
		Darjeeling.assertTrue(testBase +  2, b.inta1 == inta);

		inta = new int[10];
		_inta = inta;
		Darjeeling.assertTrue(testBase +  3, _inta == inta);
		b.inta2 = _inta;
		Darjeeling.assertTrue(testBase +  4, _inta == b.inta2);
		Darjeeling.assertTrue(testBase +  5, b.inta2 == inta);

		inta = new int[10];
		_inta = inta;
		Darjeeling.assertTrue(testBase +  6, _inta == inta);
		b.inta3 = _inta;
		Darjeeling.assertTrue(testBase +  7, _inta == b.inta3);
		Darjeeling.assertTrue(testBase +  8, b.inta3 == inta);

	}	
	
	public static void test(int testBase)
	{
		testInteger(testBase + 00);
		testShort(testBase + 10);		
		testByte(testBase + 20);		
		testBoolean(testBase + 30);
		testIntArray(testBase + 40);
	}
	

}
