/*
 * opcodes.c
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 

#define JVM_NOP 0
#define JVM_SCONST_M1 1
#define JVM_SCONST_0 2
#define JVM_SCONST_1 3
#define JVM_SCONST_2 4
#define JVM_SCONST_3 5
#define JVM_SCONST_4 6
#define JVM_SCONST_5 7
#define JVM_ICONST_M1 8
#define JVM_ICONST_0 9
#define JVM_ICONST_1 10
#define JVM_ICONST_2 11
#define JVM_ICONST_3 12
#define JVM_ICONST_4 13
#define JVM_ICONST_5 14
#define JVM_ACONST_NULL 15
#define JVM_BSPUSH 16
#define JVM_BIPUSH 17
#define JVM_SSPUSH 18
#define JVM_SIPUSH 19
#define JVM_IIPUSH 20
#define JVM_LDS 21
#define JVM_SLOAD 22
#define JVM_SLOAD_0 23
#define JVM_SLOAD_1 24
#define JVM_SLOAD_2 25
#define JVM_SLOAD_3 26
#define JVM_ILOAD 27
#define JVM_ILOAD_0 28
#define JVM_ILOAD_1 29
#define JVM_ILOAD_2 30
#define JVM_ILOAD_3 31
#define JVM_ALOAD 32
#define JVM_ALOAD_0 33
#define JVM_ALOAD_1 34
#define JVM_ALOAD_2 35
#define JVM_ALOAD_3 36
#define JVM_SSTORE 37
#define JVM_SSTORE_0 38
#define JVM_SSTORE_1 39
#define JVM_SSTORE_2 40
#define JVM_SSTORE_3 41
#define JVM_ISTORE 42
#define JVM_ISTORE_0 43
#define JVM_ISTORE_1 44
#define JVM_ISTORE_2 45
#define JVM_ISTORE_3 46
#define JVM_ASTORE 47
#define JVM_ASTORE_0 48
#define JVM_ASTORE_1 49
#define JVM_ASTORE_2 50
#define JVM_ASTORE_3 51
#define JVM_BALOAD 52
#define JVM_CALOAD 53
#define JVM_SALOAD 54
#define JVM_IALOAD 55
#define JVM_AALOAD 56
#define JVM_BASTORE 57
#define JVM_CASTORE 58
#define JVM_SASTORE 59
#define JVM_IASTORE 60
#define JVM_AASTORE 61
#define JVM_IPOP 62
#define JVM_IPOP2 63
#define JVM_IDUP 64
#define JVM_IDUP2 65
#define JVM_IDUP_X1 66
#define JVM_ISWAP_X 67
#define JVM_APOP 68
#define JVM_APOP2 69
#define JVM_ADUP 70
#define JVM_ADUP2 71
#define JVM_ADUP_X1 72
#define JVM_ADUP_X2 73
#define JVM_ASWAP 74
#define JVM_GETFIELD_B 75
#define JVM_GETFIELD_C 76
#define JVM_GETFIELD_S 77
#define JVM_GETFIELD_I 78
#define JVM_GETFIELD_A 79
#define JVM_PUTFIELD_B 80
#define JVM_PUTFIELD_C 81
#define JVM_PUTFIELD_S 82
#define JVM_PUTFIELD_I 83
#define JVM_PUTFIELD_A 84
#define JVM_GETSTATIC_B 85
#define JVM_GETSTATIC_C 86
#define JVM_GETSTATIC_S 87
#define JVM_GETSTATIC_I 88
#define JVM_GETSTATIC_A 89
#define JVM_PUTSTATIC_B 90
#define JVM_PUTSTATIC_C 91
#define JVM_PUTSTATIC_S 92
#define JVM_PUTSTATIC_I 93
#define JVM_PUTSTATIC_A 94
#define JVM_SADD 95
#define JVM_SSUB 96
#define JVM_SMUL 97
#define JVM_SDIV 98
#define JVM_SREM 99
#define JVM_SNEG 100
#define JVM_SSHL 101
#define JVM_SSHR 102
#define JVM_SUSHR 103
#define JVM_SAND 104
#define JVM_SOR 105
#define JVM_SXOR 106
#define JVM_IADD 107
#define JVM_ISUB 108
#define JVM_IMUL 109
#define JVM_IDIV 110
#define JVM_IREM 111
#define JVM_INEG 112
#define JVM_ISHL 113
#define JVM_ISHR 114
#define JVM_IUSHR 115
#define JVM_IAND 116
#define JVM_IOR 117
#define JVM_IXOR 118
#define JVM_BINC 119
#define JVM_SINC 120
#define JVM_IINC 121
#define JVM_S2B 122
#define JVM_S2I 123
#define JVM_I2B 124
#define JVM_I2S 125
#define JVM_IIFEQ 126
#define JVM_IIFNE 127
#define JVM_IIFLT 128
#define JVM_IIFGE 129
#define JVM_IIFGT 130
#define JVM_IIFLE 131
#define JVM_IFNULL 132
#define JVM_IFNONNULL 133
#define JVM_IF_SCMPEQ 134
#define JVM_IF_SCMPNE 135
#define JVM_IF_SCMPLT 136
#define JVM_IF_SCMPGE 137
#define JVM_IF_SCMPGT 138
#define JVM_IF_SCMPLE 139
#define JVM_IF_ICMPEQ 140
#define JVM_IF_ICMPNE 141
#define JVM_IF_ICMPLT 142
#define JVM_IF_ICMPGE 143
#define JVM_IF_ICMPGT 144
#define JVM_IF_ICMPLE 145
#define JVM_IF_ACMPEQ 146
#define JVM_IF_ACMPNE 147
#define JVM_GOTO 148
#define JVM_ GOTO_W 149
#define JVM_TABLESWITCH 150
#define JVM_LOOKUPSWITCH 151
#define JVM_SRETURN 152
#define JVM_IRETURN 153
#define JVM_ARETURN 154
#define JVM_RETURN 155
#define JVM_INVOKEVIRTUAL 156
#define JVM_INVOKESPECIAL 157
#define JVM_INVOKESTATIC 158
#define JVM_INVOKEINTERFACE 159
#define JVM_NEW 160
#define JVM_NEWARRAY 161
#define JVM_ANEWARRAY 162
#define JVM_ARRAYLENGTH 163
#define JVM_ATHROW 164
#define JVM_CHECKCAST 165
#define JVM_INSTANCEOF 166
#define JVM_MONITORENTER 167
#define JVM_MONITOREXIT 168
#define JVM_IDUP_X2 169
#define JVM_IINC_W 170
#define JVM_SINC_W 171
#define JVM_I2C 172
#define JVM_S2C 173
#define JVM_B2C 174
#define JVM_IDUP_X 175
#define JVM_SIFEQ 176
#define JVM_SIFNE 177
#define JVM_SIFLT 178
#define JVM_SIFGE 179
#define JVM_SIFGT 180
#define JVM_SIFLE 181
#define JVM_LCONST_0 182
#define JVM_LCONST_1 183
#define JVM_LLOAD 184
#define JVM_LLOAD_0 185
#define JVM_LLOAD_1 186
#define JVM_LLOAD_2 187
#define JVM_LLOAD_3 188
#define JVM_LLPUSH 189
#define JVM_LSTORE 190
#define JVM_LSTORE_0 191
#define JVM_LSTORE_1 192
#define JVM_LSTORE_2 193
#define JVM_LSTORE_3 194
#define JVM_LALOAD 195
#define JVM_LASTORE 196
#define JVM_GETFIELD_L 197
#define JVM_PUTFIELD_L 198
#define JVM_GETSTATIC_L 199
#define JVM_PUTSTATIC_L 200
#define JVM_LADD 201
#define JVM_LSUB 202
#define JVM_LMUL 203
#define JVM_LDIV 204
#define JVM_LREM 205
#define JVM_LNEG 206
#define JVM_LSHL 207
#define JVM_LSHR 208
#define JVM_LUSHR 209
#define JVM_LAND 210
#define JVM_LOR 211
#define JVM_LXOR 212
#define JVM_LRETURN 213
#define JVM_L2I 214
#define JVM_L2S 215
#define JVM_I2L 216
#define JVM_S2L 217
#define JVM_LCMP 218

#ifdef DARJEELING_DEBUG

const char *jvm_opcodes[] = {
"nop",
"sconst_m1",
"sconst_0",
"sconst_1",
"sconst_2",
"sconst_3",
"sconst_4",
"sconst_5",
"iconst_m1",
"iconst_0",
"iconst_1",
"iconst_2",
"iconst_3",
"iconst_4",
"iconst_5",
"aconst_null",
"bspush",
"bipush",
"sspush",
"sipush",
"iipush",
"lds",
"sload",
"sload_0",
"sload_1",
"sload_2",
"sload_3",
"iload",
"iload_0",
"iload_1",
"iload_2",
"iload_3",
"aload",
"aload_0",
"aload_1",
"aload_2",
"aload_3",
"sstore",
"sstore_0",
"sstore_1",
"sstore_2",
"sstore_3",
"istore",
"istore_0",
"istore_1",
"istore_2",
"istore_3",
"astore",
"astore_0",
"astore_1",
"astore_2",
"astore_3",
"baload",
"caload",
"saload",
"iaload",
"aaload",
"bastore",
"castore",
"sastore",
"iastore",
"aastore",
"ipop",
"ipop2",
"idup",
"idup2",
"idup_x1",
"iswap_x",
"apop",
"apop2",
"adup",
"adup2",
"adup_x1",
"adup_x2",
"aswap",
"getfield_b",
"getfield_c",
"getfield_s",
"getfield_i",
"getfield_a",
"putfield_b",
"putfield_c",
"putfield_s",
"putfield_i",
"putfield_a",
"getstatic_b",
"getstatic_c",
"getstatic_s",
"getstatic_i",
"getstatic_a",
"putstatic_b",
"putstatic_c",
"putstatic_s",
"putstatic_i",
"putstatic_a",
"sadd",
"ssub",
"smul",
"sdiv",
"srem",
"sneg",
"sshl",
"sshr",
"sushr",
"sand",
"sor",
"sxor",
"iadd",
"isub",
"imul",
"idiv",
"irem",
"ineg",
"ishl",
"ishr",
"iushr",
"iand",
"ior",
"ixor",
"binc",
"sinc",
"iinc",
"s2b",
"s2i",
"i2b",
"i2s",
"ifeq",
"ifne",
"iflt",
"ifge",
"ifgt",
"ifle",
"ifnull",
"ifnonnull",
"if_scmpeq",
"if_scmpne",
"if_scmplt",
"if_scmpge",
"if_scmpgt",
"if_scmple",
"if_icmpeq",
"if_icmpne",
"if_icmplt",
"if_icmpge",
"if_icmpgt",
"if_icmple",
"if_acmpeq",
"if_acmpne",
"goto",
"goto_w",
"tableswitch",
"lookupswitch",
"sreturn",
"ireturn",
"areturn",
"return",
"invokevirtual",
"invokespecial",
"invokestatic",
"invokeinterface",
"new",
"newarray",
"anewarray",
"arraylength",
"athrow",
"checkcast",
"instanceof",
"monitorenter",
"monitorexit",
"idup_x2",
"iinc_w",
"sinc_w",
"i2c",
"s2c",
"b2c",
"idup_x",
"sifeq",
"sifne",
"siflt",
"sifge",
"sifgt",
"sifle",
"lconst_0",
"lconst_1",
"lload",
"lload_0",
"lload_1",
"lload_2",
"lload_3",
"llpush",
"lstore",
"lstore_0",
"lstore_1",
"lstore_2",
"lstore_3",
"laload",
"lastore",
"getfield_l",
"putfield_l",
"getstatic_l",
"putstatic_l",
"ladd",
"lsub",
"lmul",
"ldiv",
"lrem",
"lneg",
"lshl",
"lshr",
"lushr",
"land",
"lor",
"lxor",
"lreturn",
"l2i",
"l2s",
"i2l",
"s2l",
"lcmp"
};
#endif
