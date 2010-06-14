/*
 * Math.java
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
 

package java.lang;

public final strictfp class Math {

    /**
     * Don't let anyone instantiate this class.
     */
    private Math() {}

    /**
     * Returns the absolute value of an <code>int</code> value.
     * If the argument is not negative, the argument is returned.
     * If the argument is negative, the negation of the argument is returned.
     * <p>
     * Note that if the argument is equal to the value of
     * <code>Integer.MIN_VALUE</code>, the most negative representable
     * <code>int</code> value, the result is that same value, which is
     * negative.
     *
     * @param   a   an <code>int</code> value.
     * @return  the absolute value of the argument.
     * @see     java.lang.Integer#MIN_VALUE
     */
    public static int abs(int a) {
        return (a < 0) ? -a : a;
    }

    /**
     * Returns the absolute value of a <code>long</code> value.
     * If the argument is not negative, the argument is returned.
     * If the argument is negative, the negation of the argument is returned.
     * <p>
     * Note that if the argument is equal to the value of
     * <code>Long.MIN_VALUE</code>, the most negative representable
     * <code>long</code> value, the result is that same value, which is
     * negative.
     *
     * @param   a   a <code>long</code> value.
     * @return  the absolute value of the argument.
     * @see     java.lang.Long#MIN_VALUE
     */
//    public static long abs(long a) {
//        return (a < 0) ? -a : a;
//    }

    /**
     * Returns the greater of two <code>int</code> values. That is, the
     * result is the argument closer to the value of
     * <code>Integer.MAX_VALUE</code>. If the arguments have the same value,
     * the result is that same value.
     *
     * @param   a   an <code>int</code> value.
     * @param   b   an <code>int</code> value.
     * @return  the larger of <code>a</code> and <code>b</code>.
     * @see     java.lang.Long#MAX_VALUE
     */
    public static int max(int a, int b) {
        return (a >= b) ? a : b;
    }

    /**
     * Returns the greater of two <code>long</code> values. That is, the
     * result is the argument closer to the value of
     * <code>Long.MAX_VALUE</code>. If the arguments have the same value,
     * the result is that same value.
     *
     * @param   a   a <code>long</code> value.
     * @param   b   a <code>long</code> value.
     * @return  the larger of <code>a</code> and <code>b</code>.
     * @see     java.lang.Long#MAX_VALUE
     */
//    public static long max(long a, long b) {
//        return (a >= b) ? a : b;
//    }

    /**
     * Returns the smaller of two <code>int</code> values. That is, the
     * result the argument closer to the value of <code>Integer.MIN_VALUE</code>.
     * If the arguments have the same value, the result is that same value.
     *
     * @param   a   an <code>int</code> value.
     * @param   b   an <code>int</code> value.
     * @return  the smaller of <code>a</code> and <code>b</code>.
     * @see     java.lang.Long#MIN_VALUE
     */
    public static int min(int a, int b) {
        return (a <= b) ? a : b;
    }

    /**
     * Returns the smaller of two <code>long</code> values. That is, the
     * result is the argument closer to the value of
     * <code>Long.MIN_VALUE</code>. If the arguments have the same value,
     * the result is that same value.
     *
     * @param   a   a <code>long</code> value.
     * @param   b   a <code>long</code> value.
     * @return  the smaller of <code>a</code> and <code>b</code>.
     * @see     java.lang.Long#MIN_VALUE
     */
//    public static long min(long a, long b) {
//        return (a <= b) ? a : b;
//    }

}
