/* 
 * File:   external_eep.h
 * Author: NIKHIL
 *
 * Created on 7 May, 2024, 7:52 PM
 */

#ifndef AT24C04_H
#define	AT24C04_H     

#define SLAVE_WRITE_EE            0xA0 // 0xD0
#define SLAVE_READ_EE             0xA1 // 0xD1

#define SEC_ADDR                0x00



void at24c04(void);
unsigned char eeprom_at24c04_read(unsigned char memory_loc);
void eeprom_at24c04_byte_write(unsigned char memory_loc, unsigned char data);
void eeprom_at24c04_str_write(unsigned char memory_loc, char *data);

#endif	/* DS1307_H */

