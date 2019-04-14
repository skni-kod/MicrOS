#include "../math.h"

double nextafter(double x, double y)
{
    double_byte dbyte_given;
    dbyte_given.f = x;
    unsigned char carry = 0;
    unsigned char borrow = 0;

    if(x == y)
    {
        return y;
    }

    // Increment positive and decrement negative
    if((y > x && x >= 0) || (y < x && x <= 0))
    {
        // Make sure sign is good sign
        if(y > x && x >= 0)
        {
            dbyte_given.f = copysign(dbyte_given.f, 1.0);
        }
        if(y < x && x <= 0)
        {
            dbyte_given.f = copysign(dbyte_given.f, -1.0);
        }
        // Add one bite
        dbyte_given.bytes[0]++;
        if(dbyte_given.bytes[0] == 0)
        {
            carry = 1;
        }
        // Handle carry if exist through from 1 to 5 byte
        for(int i = 1 ; i < 6; i++)
        {
            // Do only if we have carry byte set
            if(carry == 1)
            {
                // Increment
                dbyte_given.bytes[i]++;
                // Clear carry byte
                carry = 0;
                // If there was overflow set carry again for next iteration
                if(dbyte_given.bytes[i] == 0)
                {
                    carry = 1;
                }
            }
        }

        // We still have carry bit add it to remaining 4 bits
        if(carry == 1)
        {
            // Get last 4 bytes of mantissa
            unsigned char last_4_bytes_of_mantisa = dbyte_given.bytes[6] & 0x0F;
            // If it's lower than 15 add carry
            if (last_4_bytes_of_mantisa < 15)
            {
                last_4_bytes_of_mantisa++;
                carry = 0;
                dbyte_given.bytes[6] &= 0xF0;
                dbyte_given.bytes[6] |= last_4_bytes_of_mantisa;
            }
            // Increase exponent and clear mantisa
            else
            {
                // Clear mantisa
                for(int i = 0 ; i < 6; i++)
                {
                    dbyte_given.bytes[i] = 0;
                }
                dbyte_given.bytes[6] &= 0xF0;

                // Increase exponent
                unsigned char first_4_bytes_of_exponent = dbyte_given.bytes[6] & 0xF0;
                if(first_4_bytes_of_exponent < 240)
                {
                    // Increase last 4 bits
                    first_4_bytes_of_exponent >>= 4;
                    first_4_bytes_of_exponent++;
                    first_4_bytes_of_exponent <<= 4;
                    first_4_bytes_of_exponent &= 0xF0;
                    dbyte_given.bytes[6] &= 0x0F;
                    dbyte_given.bytes[6] |= first_4_bytes_of_exponent;
                }
                // We have to increase first bytes of exponent
                else
                {
                    // Clear first 4 bytes of exponent
                    dbyte_given.bytes[6] &= 0x0F;
                    // Increase last bytes
                    unsigned char last_7_bytes_of_exponent = dbyte_given.bytes[7] & 0x7F;
                    if(last_7_bytes_of_exponent < 127)
                    {
                        // Increase last 7 bits
                        dbyte_given.bytes[7]++;
                    }
                }
            }    
        }
    }
    // Decrement positive and increment negative
    else if((y < x && x >= 0) || (y > x && x <= 0))
    {
        // Make sure sign is good sign
        if(y < x && x >= 0)
        {
            dbyte_given.f = copysign(dbyte_given.f, 1.0);
        }
        if(y > x && x <= 0)
        {
            dbyte_given.f = copysign(dbyte_given.f, -1.0);
        }
        // Check if we can subtract
        if(dbyte_given.bytes[0] != 0)
        {
            dbyte_given.bytes[0]--;
        }
        else
        {
            dbyte_given.bytes[0] = 255;
            borrow = 1;
        }
        // If we can't we need to borrow values
        for(int i = 1 ; i < 6; i++)
        {
            // Do only if we have borrow byte set
            if(borrow == 1)
            {
                borrow = 0;
                // If there is enough value end with borrow
                if(dbyte_given.bytes[i] != 0)
                {
                    dbyte_given.bytes[i]--;
                }
                // Borrow from next
                else
                {
                    dbyte_given.bytes[i] = 255;
                    borrow = 1;
                }
            }
        }
        // If we still have borrow set to 1
        if(borrow == 1)
        {
            // Get last 4 bytes of mantissa
            unsigned char last_4_bytes_of_mantisa = dbyte_given.bytes[6] & 0x0F;
            // If it's higher than 0 borrow one
            if (last_4_bytes_of_mantisa > 0)
            {
                last_4_bytes_of_mantisa--;
                borrow = 0;
                dbyte_given.bytes[6] &= 0xF0;
                dbyte_given.bytes[6] |= last_4_bytes_of_mantisa;
            }
            // Decrease exponent and fill mantisa with 1
            else
            {
                 // Fill mantisa
                for(int i = 0 ; i < 6; i++)
                {
                    dbyte_given.bytes[i] = 0xFF;
                }
                dbyte_given.bytes[6] |= 0x0F;

                // Decrease exponent
                unsigned char first_4_bytes_of_exponent = dbyte_given.bytes[6] & 0xF0;
                if(first_4_bytes_of_exponent > 0)
                {
                    // Decrease last 4 bits
                    first_4_bytes_of_exponent >>= 4;
                    first_4_bytes_of_exponent--;
                    first_4_bytes_of_exponent <<= 4;
                    first_4_bytes_of_exponent &= 0xF0;
                    dbyte_given.bytes[6] &= 0x0F;
                    dbyte_given.bytes[6] |= first_4_bytes_of_exponent;
                }
                // We have to decrease first bytes of exponent
                else
                {
                    // Set first 4 bytes of exponent
                    dbyte_given.bytes[6] |= 0xF0;
                    // Decrease last bytes
                    unsigned char last_7_bytes_of_exponent = dbyte_given.bytes[7] & 0x7F;
                    if(last_7_bytes_of_exponent > 0)
                    {
                        // Decrease last 7 bits
                        dbyte_given.bytes[7]--;
                    }
                    // Set all to zeros
                    else
                    {
                        for(int i = 0 ; i < 7; i++)
                        {
                            dbyte_given.bytes[i] = 0x00;
                        }
                        dbyte_given.bytes[7] &= 0x80;
                    }
                }
            }
        }
    }
    return dbyte_given.f;
}