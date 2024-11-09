/*
 * File:   external_eep.c
 * Author: NIKHIL
 *
 * Created on 7 May, 2024, 7:53 PM
 */
#include "main.h"

void init_at24c04(void)
{
    unsigned char dummy;
   
    dummy = eeprom_at24c04_read(SEC_ADDR);
    dummy = dummy & 0x7F;
    eeprom_at24c04_byte_write(SEC_ADDR, dummy);// ch = 0
}

unsigned char eeprom_at24c04_read(unsigned char memory_loc)
{
    unsigned char received_data = 0;
    
    i2c_start();
    i2c_write(SLAVE_WRITE_EE);
    i2c_write(memory_loc);
    i2c_rep_start();
    i2c_write(SLAVE_READ_EE);
    received_data = i2c_read(0);
    i2c_stop();
    
    return received_data;
}

void eeprom_at24c04_byte_write(unsigned char memory_loc, unsigned char data) // SEc_ADDR, data
{
    i2c_start();
    i2c_write(SLAVE_WRITE_EE);
    i2c_write(memory_loc);
    i2c_write(data);
    i2c_stop();
}

void eeprom_at24c04_str_write(unsigned char memory_loc, char *data)
{
    while(*data != 0)
    {
        eeprom_at24c04_byte_write(memory_loc, *data);
        data++;
        memory_loc++;
                
    }
}
