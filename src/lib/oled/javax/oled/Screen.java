package javax.oled;

public class Screen
{
	
	public static native void init();
	public static native void clear();
	public static native void vsync();

	public static native void setFill(boolean fill);
	public static native void setTextOpaque(boolean opaque);
	public static native void setFontSize(int size);
	public static native void setBackgroundColor(int colour);

	public static native void circle(int x, int y, int radius, int colour);
	public static native void rectangle(int x1, int y1, int x2, int y2, int colour);
	public static native void line(int x1, int y1, int x2, int y2, int colour);
	public static native void pixel(int x, int y, int colour);

	public static native void copy(int xs, int ys, int xd, int yd, int w, int h);
	
	public static native void putChar(char c, int x, int y, int w, int h, int colour);
	public static native void putChar(char c, int col, int row, int colour);
	public static native void putString(String str, int font, int col, int row, int colour);
	public static native void putString(String str, int font, int x, int y, int w, int h, int colour);
	
	public static void putCenteredString(String str, int font, int x, int y, int w, int h, int colour)
	{
		short tx = (short)( (w-stringWidth(str, font))/2 + x );
		short ty = (short)( (h-stringHeight(font))/2 + y );
		Screen.putString(str, font, tx, ty, 1, 1, colour); 
	}

	public static void putLeftCenteredString(String str, int font, int x, int y, int w, int h, int colour)
	{
		short ty = (short)( (h-stringHeight(font))/2 + y );
		Screen.putString(str, font, x, ty, 1, 1, colour); 
	}

	public static void putRightCenteredString(String str, int font, int x, int y, int w, int h, int colour)
	{
		short tx = (short)( (w-stringWidth(str, font)) + x );
		short ty = (short)( (h-stringHeight(font))/2 + y );
		Screen.putString(str, font, tx, ty, 1, 1, colour); 
	}

	public static short stringWidth(String str, int font)
	{
		if (font==0) return (short)(str.length() *  5);
		if (font==1) return (short)(str.length() *  8);
		if (font==2) return (short)(str.length() *  8);
		if (font==3) return (short)(str.length() * 12);
		return 0;
	}
	
	public static short stringHeight(int font)
	{
		if (font==0) return  7;
		if (font==1) return  8;
		if (font==2) return 12;
		if (font==3) return 16;
		return 0;
	}
	
	public static void putShort(short value, short font, short x, short y, short colour)
	{
		short tw = 8;
		if (font==0) tw = 5;
		if (font==3) tw = 12;
		for (short i=0; i<5; i++)
		{
			putChar((char)((value%10)+'0'), (short)(x+i*tw), y, 1, 1, 0xffff);
			value/=10;
		}
	}
	
	public static short encodeColor(int r, int g, int b)
	{
		return (short)((b>>3) | ((g>>2) << 5) | ((r>>3)<<11));
	}
	
}
