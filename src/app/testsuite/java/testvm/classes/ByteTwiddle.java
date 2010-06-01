package testvm.classes;
import javax.darjeeling.Darjeeling;

public class ByteTwiddle
{
        //Bytes and shorts interleaved
        private static byte staticByte_;
        private static short staticShort_;
        private static int staticInt_;

        private byte fieldByte_;
        private short fieldShort_;
        private int fieldInt_;

        //class fields occupies odd number of bytes. So this reference could be placed unaligned.
        private Byte[] byteArray=new Byte[3];

        //static setters/getters
        public void setStaticByte(byte value) {staticByte_ = value;}
        public byte getStaticByte() {return staticByte_;}
        public void setStaticShort(short value) {staticShort_ = value;}
        public short getStaticShort() {return staticShort_;}
        public void setStaticInt(int value) {staticInt_ = value;}
        public int getStaticInt() {return staticInt_;}

        //field setters/getters
        public void setFieldByte(byte value) {fieldByte_ = value;}
        public byte getFieldByte() {return fieldByte_;}
        public void setFieldShort(short value) {fieldShort_ = value;}
        public short getFieldShort() {return fieldShort_;}
        public void setFieldInt(int value) {fieldInt_ = value;}
        public int getFieldInt() {return fieldInt_;}

        //function arguments could be unaligned
        public int setArray(short foo, byte staticByte, byte fieldByte, byte b3, short bar)
        {
                byteArray[0] = staticByte;
                byteArray[1] = fieldByte;
                byteArray[2] = b3;
                return foo*bar;
        }
        public int getProduct()
        {
                return staticByte_*staticShort_*staticInt_*fieldByte_*fieldShort_*fieldInt_;
        }
}
