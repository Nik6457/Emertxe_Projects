/*
 * File:   main.c
 * Author: NIKHIL
 *
 * Created on 8 May, 2024, 10:16 AM
 */


#include "main.h"

#pragma config WDTE = OFF



static void init_config(void) {
   // intit i2c
    init_i2c(100000);
    
    // init rtc
    init_ds1307();
    
    // init clcd
    init_clcd();
    
    // init adc
    init_adc();
    
    // init digital keypad 
    init_digital_keypad();
    
    // init timer2
    init_timer2();
    
    // init uart
    init_uart(9600);
    
    GIE = 1;
    PEIE = 1;
}

void main(void) {
    unsigned char event[3] = "ON";
    unsigned char speed = 0; // 0-99
    unsigned char control_flag = DASH_BOARD_FLAG,menu_pos, key, reset_flag; //dashboard screen
    char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};
    unsigned char gr = 0;
    int wait = 0, i, delay = 0;
    init_config();
    
    eeprom_at24c04_str_write(0x00, "1010");
    log_event(event, speed);
    
    
    while (1) {
      
        speed = read_adc() / 10;
        if(speed > 99)
        {
            speed = 99;
        }
        
        key = read_digital_keypad(STATE);
         for(int j=100;j--;);
        while(key == SW4 )
        {
            key = read_digital_keypad(LEVEL);  
            for(int j=100;j--;);
                if(delay++ == 5000)
                {
                    delay = 0;
                    key = LPSW4;
                }
        }
        if(delay < 5000 && delay > 0)
        {
            key = SW4;
            delay = 0;
        }

        while(key == SW5 )
        {
            key = read_digital_keypad(LEVEL);  
            for(int j=100;j--;);
                if(delay++ == 5000)
                {
                    delay = 0;
                    key = LPSW5;
                }
        }
        if(delay < 5000 && delay > 0)
        {
            key = SW5;
            delay = 0;
        }
        
        for(unsigned char j = 100; j--;);
        if(key == SW1)
        {
            strcpy(event, "C ");
            log_event(event, speed);
        }
        else if(key == SW2 && gr < 6)
        {
            strcpy(event, gear[gr]);
            gr++;
            log_event(event, speed);
        }
        else if(key == SW3 && gr > 0)
        {
               gr--;
               strcpy(event, gear[gr]);
               log_event(event, speed);
        }
        
        else if ((key == SW4 || key == SW5) && control_flag == DASH_BOARD_FLAG)
        {
            control_flag = LOGIN_FLAG;
            clear_screen();
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(4),INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            reset_flag = RESET_PASSWORD;
            TMR2ON = 1;
        }
        else if(control_flag == LOGIN_MENU_FLAG && (key == LPSW4))
        {
            switch(menu_pos)
            {
                case 0:
                    clear_screen();
                    clcd_print("# TIME     E  SP", LINE1(0));
                    control_flag = VIEW_LOG_FLAG;
                    reset_flag = RESET_VIEW_LOG_POS;
                    break;
                case 1:
                    control_flag = DOWNLOAD_LOG_FLAG;
                    break;
                case 2:
                    log_event("CL",speed);
                    clear_screen();
                    control_flag=CLEAR_LOG_FLAG;
                    reset_flag=RESET_MEMORY;
                    break;
                case 3:
                    log_event("CT",speed);
					clear_screen();
					control_flag = SET_TIME_FLAG;
					reset_flag = RESET_TIME;
					break;
                case 4:
					clear_screen();
					control_flag = CHANGE_PASSWORD_FLAG;
                    reset_flag = RESET_PASSWORD;
                    clcd_print("Enter Password", LINE1(1));
                    clcd_write(LINE2(4), INST_MODE);
                    clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
					break;
                    
            }
        }
       else if ( key == LPSW5 && (control_flag == LOGIN_MENU_FLAG ||control_flag == VIEW_LOG_FLAG ))
        {
                control_flag = DASH_BOARD_FLAG;
                clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
        }
      
         else if ( key == LPSW4 && (control_flag == VIEW_LOG_FLAG ))
        {
                control_flag = LOGIN_MENU_FLAG;
                clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
        }
     
        switch(control_flag)
        {
            case DASH_BOARD_FLAG:
                   display_dashboard(event, speed);
                   break;
            case LOGIN_FLAG:
                switch(login(key, reset_flag))
                {
                    case RETURN_BACK:
                        clear_screen();
                        control_flag = DASH_BOARD_FLAG;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        TMR2ON = 0;
                        break;
                    case LOGIN_SUCCESS:
                        control_flag = LOGIN_MENU_FLAG;
                        reset_flag = RESET_LOGIN_MENU;
                        continue;
                        break;
                }
                break;
            case LOGIN_MENU_FLAG:
               menu_pos = menu_display(key, reset_flag);
               break;
            case VIEW_LOG_FLAG:
                view_log(key, reset_flag);
                break;
            case DOWNLOAD_LOG_FLAG:
                    download_log();
                    clear_screen();
					control_flag = LOGIN_MENU_FLAG;
					reset_flag = RESET_LOGIN_MENU;
                break;
            case CLEAR_LOG_FLAG:
                if(clear_log(reset_flag) == TASK_SUCCESS)
                {
                        clear_screen();
                        control_flag=LOGIN_MENU_FLAG;
                        reset_flag=RESET_LOGIN_MENU;
                        continue; 
                }
                break;
            case SET_TIME_FLAG:
                if(change_time(key, reset_flag) == TASK_SUCCESS)
                    wait = 100;
                if(wait > 0 && --wait == 0)
                {
                    control_flag = LOGIN_MENU_FLAG;
                    reset_flag = RESET_LOGIN_MENU;
                    clear_screen();
                    continue;
                }
           
                break;
            case CHANGE_PASSWORD_FLAG:
                   control_flag = change_password(key,reset_flag);
                   
                    break; 
        }
        
        reset_flag = RESET_NOTHING;

    }
    return;
}
