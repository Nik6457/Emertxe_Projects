/* 
 * File:   car_black_box.h
 * Author: NIKHIL
 *
 * Created on 8 May, 2024, 8:08 PM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

void display_dashboard(unsigned char event[], unsigned char speed);
void log_event(unsigned char event[], unsigned char speed);
void clear_screen(void);
unsigned char login(unsigned char key, unsigned char reset_flag);
unsigned char menu_display(unsigned char key, unsigned char reset_flag);
void view_log(unsigned char key, unsigned char reset_flag);
void download_log(void);
char clear_log(unsigned char reset_flag);
char change_time(unsigned char key, unsigned char reset_flag);
unsigned char change_password(unsigned char key,unsigned char reset_flag);
#endif	/* CAR_BLACK_BOX_H */

