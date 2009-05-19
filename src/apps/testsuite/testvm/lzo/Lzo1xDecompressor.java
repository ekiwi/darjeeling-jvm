package testvm.lzo;

import static testvm.lzo.Constants.LZO_E_ERROR;
import static testvm.lzo.Constants.LZO_E_INPUT_NOT_CONSUMED;
import static testvm.lzo.Constants.LZO_E_INPUT_OVERRUN;
import static testvm.lzo.Constants.LZO_E_LOOKBEHIND_OVERRUN;
import static testvm.lzo.Constants.LZO_E_OK;

public final class Lzo1xDecompressor
{
    public Lzo1xDecompressor() { }

    private static final String copyright = "LZO Copyright (C) 1996-1999 Markus F.X.J. Oberhumer <markus.oberhumer@jk.uni-linz.ac.at>";

    private static final int U(byte b) {
        return b & 0xff;
    }

    /**
     * Decompress from array src to array dst.
     * The arrays may overlap under certain conditions (see LZO.FAQ).
     *
     * @param  src      the compressed data
     * @param  src_off  starting index of compressed data in array src
     * @param  src_len  length of compressed data in array src
     * @param  dst      where the uncompressed data will be stored
     * @param  dst_off  starting index of uncompressed data in array dst
     * @param  dst_len  will be set to the length of the uncompressed data
     * @return an error code (see <code>org.lzo.Constants</code>)
     */
    public final int decompress ( byte src[], int src_off, int src_len,byte dst[], int dst_off, Int dst_len )
    {
        int ip = src_off;
        int op = dst_off;
        int t;
        int m_pos;

        t = U(src[ip++]);
        if (t > 17) {
            t -= 17;
            do dst[op++] = src[ip++]; while (--t > 0);
            t = U(src[ip++]);
            if (t < 16) return LZO_E_ERROR;
        }

    loop:
        for ( ; ; t = U(src[ip++])) {
            if (t < 16) {
                if (t == 0) {
                    while (src[ip] == 0) {
                        t += 255; ip++;
                    }
                    t += 15 + U(src[ip++]);
                }
                t += 3; do dst[op++] = src[ip++]; while (--t > 0);
                t = U(src[ip++]);
                if (t < 16) {
                    m_pos = op - 0x801 - (t >> 2) - (U(src[ip++]) << 2);
                    if (m_pos < dst_off) { t = LZO_E_LOOKBEHIND_OVERRUN; break loop; }
                    t = 3; do dst[op++] = dst[m_pos++]; while (--t > 0);
                    t = src[ip-2] & 3;
                    if (t == 0)
                        continue;
                    do dst[op++] = src[ip++]; while (--t > 0);
                    t = U(src[ip++]);
                }
            }
            for ( ; ; t = U(src[ip++])) {
                if (t >= 64) {
                    m_pos = op - 1 - ((t >> 2) & 7) - (U(src[ip++]) << 3);
                    t = (t >> 5) - 1;
                } else if (t >= 32) {
                    t &= 31;
                    if (t == 0) {
                        while (src[ip] == 0) {
                            t += 255; ip++;
                        }
                        t += 31 + U(src[ip++]);
                    }
                    m_pos = op - 1 - (U(src[ip++]) >> 2);
                    m_pos -= (U(src[ip++]) << 6);
                } else if (t >= 16) {
                    m_pos = op - ((t & 8) << 11);
                    t &= 7;
                    if (t == 0) {
                        while (src[ip] == 0) {
                            t += 255; ip++;
                        }
                        t += 7 + U(src[ip++]);
                    }
                    m_pos -= (U(src[ip++]) >> 2);
                    m_pos -= (U(src[ip++]) << 6);
                    if (m_pos == op)
                        break loop;
                    m_pos -= 0x4000;
                } else {
                    m_pos = op - 1 - (t >> 2) - (U(src[ip++]) << 2);
                    t = 0;
                }
                if (m_pos < dst_off) { t = LZO_E_LOOKBEHIND_OVERRUN; break loop; }
                t += 2; do dst[op++] = dst[m_pos++]; while (--t > 0);
                t = src[ip-2] & 3;
                if (t == 0)
                    break;
                do dst[op++] = src[ip++]; while (--t > 0);
            }
        }

        ip -= src_off;
        op -= dst_off;
        dst_len.setValue(op);
        if (t < 0)
            return t;
        if (ip < src_len)
            return LZO_E_INPUT_NOT_CONSUMED;
        if (ip > src_len)
            return LZO_E_INPUT_OVERRUN;
        if (t != 1)
            return LZO_E_ERROR;
        return LZO_E_OK;
    }

}
