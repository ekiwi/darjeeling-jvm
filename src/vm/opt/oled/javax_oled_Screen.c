#include <stdint.h>

#include "fos/fos_msg.h"

#include "common/array.h"
#include "common/execution/execution.h"

#include "oled.h"

// void javax.oled.Screen.init()
void javax_oled_Screen_void_init()
{
	fos_oled_init();
}

// void javax.oled.Screen.clear()
void javax_oled_Screen_void_clear()
{
	fos_oled_cls();
}

// void javax.oled.Screen.vsync()
void javax_oled_Screen_void_vsync()
{
	fos_oled_vsync();
}

// void javax.oled.Screen.setFill(boolean)
void javax_oled_Screen_void_setFill_boolean()
{
	int32_t value = dj_exec_stackPopInt();
	fos_oled_setFill(value);
}

// void javax.oled.Screen.setTextOpaque(boolean)
void javax_oled_Screen_void_setTextOpaque_boolean()
{
	int32_t value = dj_exec_stackPopInt();
	fos_oled_setTextOpaque(value);
}

// void javax.oled.Screen.setFontSize(int)
void javax_oled_Screen_void_setFontSize_int()
{
	int32_t size = dj_exec_stackPopInt();
	fos_oled_setFontSize(size);
}

// void javax.oled.Screen.setBackgroundColor(int)
void javax_oled_Screen_void_setBackgroundColor_int()
{
	int32_t colour = dj_exec_stackPopInt();
	fos_oled_setBackground(colour);
}

// void javax.oled.Screen.circle(int, int, int, int)
void javax_oled_Screen_void_circle_int_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t radius = dj_exec_stackPopInt();
	int32_t y = dj_exec_stackPopInt();
	int32_t x = dj_exec_stackPopInt();
	fos_oled_circle(x, y, radius, colour);
}

// void javax.oled.Screen.rectangle(int, int, int, int, int)
void javax_oled_Screen_void_rectangle_int_int_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t y2 = dj_exec_stackPopInt();
	int32_t x2 = dj_exec_stackPopInt();
	int32_t y1 = dj_exec_stackPopInt();
	int32_t x1 = dj_exec_stackPopInt();
	fos_oled_rectangle(x1, y1, x2, y2, colour);

}

// void javax.oled.Screen.line(int, int, int, int, int)
void javax_oled_Screen_void_line_int_int_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t y2 = dj_exec_stackPopInt();
	int32_t x2 = dj_exec_stackPopInt();
	int32_t y1 = dj_exec_stackPopInt();
	int32_t x1 = dj_exec_stackPopInt();
	fos_oled_line(x1, y1, x2, y2, colour);
}

// void javax.oled.Screen.pixel(int, int, int)
void javax_oled_Screen_void_pixel_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t y = dj_exec_stackPopInt();
	int32_t x = dj_exec_stackPopInt();
	fos_oled_pixel(x, y, colour);
}

// void javax.oled.Screen.copy(int, int, int, int, int, int)
void javax_oled_Screen_void_copy_int_int_int_int_int_int()
{
	int32_t h = dj_exec_stackPopInt();
	int32_t w = dj_exec_stackPopInt();
	int32_t yd = dj_exec_stackPopInt();
	int32_t xd = dj_exec_stackPopInt();
	int32_t ys = dj_exec_stackPopInt();
	int32_t xs = dj_exec_stackPopInt();
	fos_oled_copy(xs, ys, xd, yd, w, h);
}

// void javax.oled.Screen.putChar(char, int, int, int, int, int)
void javax_oled_Screen_void_putChar_char_int_int_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t h = dj_exec_stackPopInt();
	int32_t w = dj_exec_stackPopInt();
	int32_t y = dj_exec_stackPopInt();
	int32_t x = dj_exec_stackPopInt();
	int32_t c = dj_exec_stackPopInt();
	fos_oled_putChar((char)c, x, y, w, h, colour);
}

// void javax.oled.Screen.putChar(char, int, int, int)
void javax_oled_Screen_void_putChar_char_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t row = dj_exec_stackPopInt();
	int32_t col = dj_exec_stackPopInt();
	int32_t c = dj_exec_stackPopInt();
	fos_oled_putCharF((char)c, col, row, colour);
}

// void javax.oled.Screen.putString(java.lang.String, int, int, int, int)
void javax_oled_Screen_void_putString_java_lang_String_int_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t row = dj_exec_stackPopInt();
	int32_t col = dj_exec_stackPopInt();
	int32_t font = dj_exec_stackPopInt();
	char *str = dj_exec_stackPopRef();
	fos_oled_putStringF(str, font, col, row, colour);
}

// void javax.oled.Screen.putString(java.lang.String, int, int, int, int, int, int)
void javax_oled_Screen_void_putString_java_lang_String_int_int_int_int_int_int()
{
	int32_t colour = dj_exec_stackPopInt();
	int32_t h = dj_exec_stackPopInt();
	int32_t w = dj_exec_stackPopInt();
	int32_t y = dj_exec_stackPopInt();
	int32_t x = dj_exec_stackPopInt();
	int32_t font = dj_exec_stackPopInt();
	char *str = dj_exec_stackPopRef();
	fos_oled_putString(str, font, x, y, w, h, colour);
}

// int javax.oled.TouchScreen._poll()
void javax_oled_TouchScreen_int__poll()
{
	dj_exec_stackPushInt(fos_oled_pollTouchScreen());
}
