using System;

namespace oscardata
{
    class Crc
    {
        UInt16 reg16 = 0xffff;        // Schieberegister

        UInt16 crc16_bytecalc(Byte byt)
        {
            int i;
            UInt16 polynom = 0x8408;      // Generatorpolynom

            for (i = 0; i < 8; ++i)
            {
                if ((reg16 & 1) != (byt & 1))
                    reg16 = (UInt16)((reg16 >> 1) ^ polynom);
                else
                    reg16 >>= 1;
                byt >>= 1;
            }
            return reg16;           // inverses Ergebnis, MSB zuerst
        }

        public UInt16 crc16_messagecalc(Byte[] data, int len)
        {
            int i;

            reg16 = 0xffff;                 // Initialisiere Shift-Register mit Startwert
            for (i = 0; i < len; i++)
            {
                reg16 = crc16_bytecalc(data[i]);        // Berechne fuer jeweils 8 Bit der Nachricht
            }
            return reg16;
        }
    }
}
