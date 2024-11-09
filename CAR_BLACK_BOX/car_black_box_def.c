/* File:   car_black_box_definition*/

#include "main.h"

unsigned char clock_reg[3];
char time[9];
char log[11];
int pos = -1;
int sec;
char overflow = 0;
int access = -1;
int return_time;
char *menu[] = {"View log", "Download log", "Clear log", "Set time", "Change Passwd"};
/*Display dashboard function*/
void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
   
    /* To store the time in HH:MM:SS format */
  // "HH:MM:SS"
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
  
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
  
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
    
    
}



void display_time(void)
{
    get_time();
    clcd_putch(time[0], LINE2(2));
    clcd_putch(time[1], LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(time[2], LINE2(5));
    clcd_putch(time[3], LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(time[4], LINE2(8));
    clcd_putch(time[5], LINE2(9));
    
}


void display_dashboard(unsigned char event[], unsigned char speed)
{
    clcd_print("TIME     E  SP",LINE1(2));
    
        
    //display time
    display_time();
    
    //display event
    clcd_print(event, LINE2(11));
    
    //display speed
    //speed = 0 ;
    clcd_putch(speed/10 + '0', LINE2(14));
    clcd_putch(speed%10 + '0', LINE2(15));
}
void log_car_event()
{
    char addr = 0x05;
    pos++;
    if(pos == 10)
    {
        pos = 0;
      
    }
    addr = pos * 10 + 5;
   
     eeprom_at24c04_str_write(addr,log);
    
  if(access < 9)
    {
        access++;
    }
}

void log_event(unsigned char event[], unsigned char speed)
{
    get_time();
    strncpy(log, time, 6);
    strncpy(&log[6], event, 2);
    
    log[8] = speed/10 + '0';
    log[9] = speed%10 + '0';
    
    log[10] = '\0';
    log_car_event();
}

unsigned char login(unsigned char key, unsigned char reset_flag)
{
    static char upasswd[4];
    static unsigned char i;
    char spasswd[4];
    unsigned char attempt_left;
    if(reset_flag == RESET_PASSWORD)
    {
       i = 0;
       upasswd[0] = '\0';
       upasswd[1] = '\0';
       upasswd[2] = '\0';
       upasswd[3] = '\0';
       attempt_left = 3;
       key = ALL_RELEASED;
       return_time = 5;
    }
    if(return_time == 0)
       return RETURN_BACK;
    
    if(key == SW4 && i < 4)
    {
        upasswd[i] = '1';
        clcd_putch('*', LINE2(4+i));
        i++;
        return_time = 5;
    }
    else if(key == SW5 && i < 4)
    {
        upasswd[i] = '0';
        clcd_putch('*', LINE2(4+i));
        i++;
        return_time = 5;
    }
    
    if(i == 4)
    {
        for(unsigned char j = 0; j < 4; j++)
        {
            spasswd[j] = eeprom_at24c04_read(j);
        }
        
        if(strncmp(spasswd, upasswd, 4) == 0)
        {
             clear_screen();
             clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
             clcd_print("Login success", LINE1(2));
             __delay_ms(2000);
             return LOGIN_SUCCESS;
        }
        else
        {
             attempt_left--;
            
             if(attempt_left == 0)
            {
              clear_screen();
              clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
              clcd_print("You are blocked", LINE1(0));
              clcd_print("wait for ", LINE2(0));
              clcd_print("sec", LINE2(13));
              sec = 60;
              //__delay_ms(2000); 
              
              while(sec) //sec = 0;
              {
                   clcd_putch(sec/10+'0', LINE2(9));
                   clcd_putch(sec%10+'0', LINE2(10));
              }
            
              attempt_left = 3;
            }
            else
            {
              clear_screen();
              clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
              clcd_print("Wrong password", LINE1(2));
              clcd_putch(attempt_left+'0', LINE2(0));
              clcd_print("attempt left", LINE2(2));
         
              __delay_ms(2000); 
            }
             clear_screen();
             clcd_print("Enter Password", LINE1(1));
             clcd_write(LINE2(4),INST_MODE);
             clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
             i = 0;
             return_time = 5;
        }
    }
    
    
}
unsigned char menu_display(unsigned char key, unsigned char reset_flag)
{
    static unsigned char menu_pos;
    if(reset_flag == RESET_LOGIN_MENU)
    {
        clear_screen();
        menu_pos = 0;
        sec = 15;
    }
    if(key == SW4 && menu_pos > 0)
    {
        sec = 15;
        clear_screen();
        menu_pos--;
    }
    else if(key == SW5 && menu_pos < 4)
    {
        sec = 15;
        clear_screen();
        menu_pos++;
    }
    if(menu_pos == 4)
    {
    clcd_putch('*', LINE2(0));
    clcd_print(menu[menu_pos - 1],LINE1(2));
    clcd_print(menu[menu_pos],LINE2(2));
    }
    else
    {
    clcd_putch('*', LINE1(0));
    clcd_print(menu[menu_pos],LINE1(2));
    clcd_print(menu[menu_pos+1],LINE2(2));
    }
    return menu_pos;
}

void view_log(unsigned char key, unsigned char reset_flag)
{
   
    char view_log[11];
    static unsigned char view_pos;
    unsigned char addr;
    if(access == -1)
    {
        clcd_print("NO logs available", LINE2(0));
    }
    else
    {
        if(reset_flag == RESET_VIEW_LOG_POS)
        {
            view_pos = 0;
        }
        
        if(key == SW4 && view_pos > 0)
        {
           
            view_pos--;
        }
        else if(key == SW5 && view_pos < access)
        {
            view_pos++;
        }
        
        for(int i = 0; i < 10; i++)
        {
            addr = view_pos * 10 + 5;
            view_log[i] = eeprom_at24c04_read(addr + i);
        }
        
        clcd_putch(view_pos%10 +'0', LINE2(0));
        clcd_putch(view_log[0], LINE2(2));
        clcd_putch(view_log[1], LINE2(3));
        clcd_putch(':', LINE2(4));
        
        clcd_putch(view_log[2], LINE2(5));
        clcd_putch(view_log[3], LINE2(6));
        clcd_putch(':', LINE2(7));
        
        clcd_putch(view_log[4], LINE2(8));
        clcd_putch(view_log[5], LINE2(9));
        
        clcd_putch(view_log[6], LINE2(11));
        clcd_putch(view_log[7], LINE2(12));
        
        clcd_putch(view_log[8], LINE2(14));
        clcd_putch(view_log[9], LINE2(15));
            
        
    }
}
void download_log(void)
{
   int next = -1 ;
	char download_log[11];
	download_log[10] = 0;
	int download_pos = 0 ;
	unsigned char addr;
    //display if no logs available
	if ( access == -1 )
	{
		puts("No logs available");
	}
	else
	{
        //displaying all logs available
		puts("# TIME        EV  SP\n\r");
		putchar('\r');

		while (  next < access )
		{
			next++;
			download_pos = (overflow)? (pos + 1) % 10 : 0;
			download_pos = (download_pos + next) % 10;

			for ( int i = 0 ; i < 10 ; i++ )
			{
				addr = download_pos * 10 + 5 + i ;
				download_log[i] = eeprom_at24c04_read(addr );
			}
            //printing all logs in cute_com
			putchar ( next + '0');
			putchar ('  ');
            putchar ('  ');
			putchar ( download_log[0] );
			putchar ( download_log[1] );
			putchar (':');
			putchar ( download_log[2] );
			putchar ( download_log[3] );
			putchar (':');
			putchar ( download_log[4] );
			putchar ( download_log[5] );
			putchar ('  ');
            putchar ('  ');
			putchar ( download_log[6] );
			putchar ( download_log[7] );
			putchar ('  ');
            putchar ('  ');
			putchar ( download_log[8] );
			putchar ( download_log[9] );
			putchar ( '\n' );
			putchar ('\r');
		}
         clcd_print("Log Downloaded",LINE1(0));
         clcd_print("Successfully...",LINE2(0));
         __delay_ms(1500);
	}
    
}

char clear_log(unsigned char reset_flag)
{
    if(reset_flag==RESET_MEMORY )
    {
        access = -1;
        pos = -1;
        clcd_print(" Logs Cleared ",LINE1(0));
        clcd_print(" Successfully ",LINE2(0));
        __delay_ms(3000);
        return TASK_SUCCESS;
    }
    return TASK_FAIL;
}

char change_time(unsigned char key, unsigned char reset_flag)
{
    static unsigned int new_time[3];
	static int blink_pos ;
	static unsigned char wait , blink ;
	char dummy;
	static char flag = 0;
    
    if ( reset_flag == RESET_TIME )
	{
		get_time();
		new_time[0] = (time[0] & 0x0F ) * 10 + (time[1] & 0x0F) ;
		new_time[1] = (time[2]  & 0x0F ) * 10 + (time[3] & 0x0F) ;
		new_time[2] = (time[4] & 0x0F ) * 10 + (time[5] & 0x0F) ;
		clcd_print("Time (HH:MM:SS)",LINE1(0));
		blink_pos = 2;
		wait = 0;
		blink = 0;
		key = ALL_RELEASED;
		flag = 0;
	}
    
    if(flag)
    {
        return TASK_FAIL;
    }
    
    switch ( key )
	{

		case SW4 :
			new_time[blink_pos]++;
			break;
		case SW5 :
			blink_pos = (blink_pos + 1) % 3;
			break;
		case LPSW4:
            /*storing new time*/
			get_time();
			dummy = ((new_time[0] / 10 ) << 4 ) | new_time[0] % 10 ;
			clock_reg[0] = (clock_reg[0] & 0xc0) | dummy ;
			write_ds1307(HOUR_ADDR, clock_reg[0] );
            
			dummy = ((new_time[1] / 10 ) << 4 ) | new_time[1] % 10 ;
			clock_reg[1] = (clock_reg[1] & 0x80) | dummy;
			write_ds1307(MIN_ADDR, clock_reg[1] );
            
			dummy = ((new_time[2] / 10 ) << 4 ) | new_time[2] % 10 ;
			clock_reg[2] = (clock_reg[2] & 0x80) | dummy;
			write_ds1307(SEC_ADDR, clock_reg[2] );
            
			clear_screen();
			clcd_print("Time changed",LINE1(2));
			clcd_print("Successfully",LINE2(2));
            
            for(unsigned long int i = 0;i < 200000;i++);
			flag = 1;
			return  TASK_SUCCESS;
	}

	if( new_time[0]  > 23 )
		new_time[0] = 0;
	if ( new_time[1] > 59 )
		new_time[1] = 0;
	if ( new_time[2] > 59 )
		new_time[2] = 0;

	if ( wait ++ == 50 )
	{
		wait = 0;
		blink = !blink;
		clcd_putch( new_time[0] / 10 + 48, LINE2(0));
		clcd_putch( new_time[0] % 10 + 48 , LINE2(1));
		clcd_putch( ':', LINE2(2));
		clcd_putch( new_time[1] / 10 + 48, LINE2(3));
		clcd_putch( new_time[1] % 10 + 48, LINE2(4));
		clcd_putch( ':', LINE2(5));
		clcd_putch( new_time[2] / 10 + 48 , LINE2(6));
		clcd_putch( new_time[2] % 10 + 48, LINE2(7));
/*logic to blink at the current pos*/

		if ( blink)
		{
			switch( blink_pos )
			{
				case 0:
					clcd_print("  ",LINE2(0));
					break;
				case 1:
					clcd_print("  ",LINE2(3));
					break;
				case 2:
					clcd_print("  ",LINE2(6));
					break;
			}
		}
	}
	return TASK_FAIL;
}
unsigned char change_password(unsigned char key,unsigned char reset_flag)
{
   
    static char upasswd[4];
    char spasswd[4];
    static char newpasswd[4];
    static char re_enter[4];
    static char once = 1, once0 = 1;
    static unsigned char i = 0, n = 0, r = 0;

    if (reset_flag == RESET_PASSWORD) {
        i = 0;
        n = 0;
        r = 0;
        upasswd[0] = '\0';
        upasswd[1] = '\0';
        upasswd[2] = '\0';
        upasswd[3] = '\0';
        newpasswd[0] = '\0';
        newpasswd[1] = '\0';
        newpasswd[2] = '\0';
        newpasswd[3] = '\0';
        re_enter[0] = '\0';
        re_enter[1] = '\0';
        re_enter[2] = '\0';
        re_enter[3] = '\0';

        key = ALL_RELEASED;
        return_time = 5;
        once = once0 = 1;
    }

    if (return_time == 0) {
        return RETURN_BACK;
    }

    if (key == SW4 && i < 4) {
        upasswd[i] = '1';
        clcd_putch('*', LINE2(4 + i));
        i++;
        return_time = 5;
    } else if (key == SW5 && i < 4) {
        upasswd[i] = '0';
        clcd_putch('*', LINE2(4 + i));
        i++;
        return_time = 5;
    }

    if (i == 4) {
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
        for (unsigned char j = 0; j < 4; j++) {
            spasswd[j] = eeprom_at24c04_read(j);
        }
        if (strncmp(spasswd, upasswd, 4) == 0)
        {
            if(once)
            {
                clear_screen();
                clcd_print("Enter New Passwd", LINE1(0));
                clcd_write(LINE2(4), INST_MODE);
                clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
                once = 0;
            }
            else
            {
               if (key == SW4 && n < 4) {
                    newpasswd[n] = '1';
                    clcd_putch('*', LINE2(4 + n));
                    n++;
                    return_time = 5;
                } else if (key == SW5 && n < 4) {
                    newpasswd[n] = '0';
                    clcd_putch('*', LINE2(4 + n));
                    n++;
                    return_time = 5;
                } 
            }

            if (n == 4) {
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                if(once0)
                {
                    clear_screen();
                    clcd_print("Re-enter Passwd", LINE1(0));
                    clcd_write(LINE2(4), INST_MODE);
                    clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
                    once0 = 0;
                }
                else
                {
                    if (key == SW4 && r < 4) {
                        re_enter[r] = '1';
                        clcd_putch('*', LINE2(4 + r));
                        r++;
                        return_time = 5;
                    } else if (key == SW5 && r < 4) {
                        re_enter[r] = '0';
                        clcd_putch('*', LINE2(4 + r));
                        r++;
                        return_time = 5;
                    }
                }

                if (r == 4) {
                    if (strncmp(newpasswd, re_enter, 4) == 0) {
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        eeprom_at24c04_str_write(0x00, newpasswd);
                        clcd_print("Password", LINE1(2));
                        clcd_print("Changed...", LINE2(2));
                        __delay_ms(2000);
                        return LOGIN_MENU_FLAG;

                    } else {
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        clcd_print("Passwords", LINE1(0));
                        clcd_print("Not Matching", LINE2(0));
                        __delay_ms(1500);
                        clcd_print("Password", LINE1(0));
                        clcd_print("Not Changed", LINE2(0));
                        __delay_ms(1500);
                        clear_screen();
                        return LOGIN_MENU_FLAG;
                    }
                }
            }
        } 
        else {
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            clcd_print("Wrong Password", LINE1(0));
            __delay_ms(1500);
            clcd_print("Logging Out...", LINE2(2));
            __delay_ms(1500);
            clear_screen();
            return DASH_BOARD_FLAG;
        }
    }
    return CHANGE_PASSWORD_FLAG;
}


void clear_screen()
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}
