#include "mouse.h"

volatile uint8_t cycle = 0;
uint8_t mouse_data[4], tmp, device_id;
int32_t mouse_x=0, mouse_y=0, counter = 0, mouse_z_horizontal=0, mouse_z_vertical=0;
PS2_Mouse_Parameters litte_squeaker;
uint32_t current_system_clock;

void ps2mouse_init()
{
    // Check if PS/2 controller is available to write
    ps2mouse_check_buffer_status(Input);
    io_out_byte(PS2_CMD_PORT, PS2_MOUSE_CMD_ENABLE_SECOND_PORT); // Enable PS/2 second port
    
    uint8_t configurationByte = 0;
    litte_squeaker.last_click_time = 0;

    // reading the configuration byte
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
        io_out_byte(PS2_CMD_PORT, PS2_MOUSE_CMD_GIVE_ME_COMPAQ_STATUS);
    }
    if (ps2mouse_check_buffer_status(Output) == Full)
    {
        //taking compaq status
        configurationByte = io_in_byte(PS2_DATA_PORT);
    }

    // editing
    //enable IRQs sending
    configurationByte |= 0x02;

    // writing
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
    	//ask for setting compaq status
        io_out_byte(PS2_CMD_PORT, PS2_MOUSE_CMD_I_WANT_TO_SET_COMPAQ_STATUS);
    }
    
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
    	//setting compaq status
        io_out_byte(PS2_DATA_PORT, configurationByte);
    }

    //get device ID
    ps2mouse_write(PS2_MOUSE_CMD_DISABLE_REPORT); //mouse doesn't send position package
    ps2mouse_write(PS2_MOUSE_CMD_GET_DEVICE_ID);
    ps2mouse_read(&device_id);//device id is here

    //enabling scroll
    ps2mouse_scroll_init(ScrollEnable);

    //once again take mouse id - now it should be 3, i quess - scroll should be enable
    ps2mouse_write(PS2_MOUSE_CMD_GET_DEVICE_ID);
    ps2mouse_read(&device_id);

    //enabling 4th and 5th button
    ps2mouse_scroll_init(AdditionalButonsEnable);
    ps2mouse_write(PS2_MOUSE_CMD_GET_DEVICE_ID);
    
    //mouse id should be equal to 4. Probably...
    ps2mouse_read(&device_id);

    

    // Set defaults
    ps2mouse_write(PS2_MOUSE_CMD_DEFAULT); //defaults: sample rate, resolution etc

    // Enable data reporting
    ps2mouse_write(PS2_MOUSE_CMD_ENABLE_REPORT); //mouse sends again position packages

   //  setting sample rate to 0x28 (dec 40) - it seems to be optimal sample rate
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
        ps2mouse_write(PS2_MOUSE_CMD_SET_SAMPLE_RATE);
    }	
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
         ps2mouse_write(0x28);
    }
       

    // below some complicated things that i don't understand how them works
    // but...
	// this probably enables irq number in system, which is 12 for mouse
    pic_enable_irq(IRQ_NUM);                                     // IRQ Controller
    // this function below is probably sending an IRQ number somewhere, and now the system knows how important this interrupt is
    // but honestly, I'm guessing what it does
    // and also the function connects the interrupt number to the interrupt handler
    // probably
    idt_attach_interrupt_handler(IRQ_NUM, ps2mouse_irq_handler); // CPU IDT
    
}

bool ps2mouse_irq_handler()
{

    //char used to checking mouse driver
    //static char str[100] = "";
    switch (cycle)
    {
    case 0:
        //case 0 - chceck what button is pressed and controls double clicks
        cycle=1;
        ps2mouse_read(mouse_data);
        tmp = mouse_data[0];

        if(litte_squeaker.double_click)
            litte_squeaker.double_click = 0;

        if((tmp & 0x01) && (litte_squeaker.last_click_was_left))
        {
            if(timer_get_system_clock() - litte_squeaker.last_click_time < DOUBLE_CLICK_TIME)
            {
                //logger_log_info("Double click");
                litte_squeaker.double_click = 1;
            }
            litte_squeaker.last_click_time = timer_get_system_clock();
        }

        ps2mouse_set_standard_buttons(tmp);
        
        if(litte_squeaker.double_click)
            litte_squeaker.last_click_was_left = 0;

        break;

    case 1:
        //case 1 - takes data byte with x movement
        cycle=2;
        ps2mouse_read(mouse_data+1);
        break;

    case 2:
        //case 2 - takes data byte with y movement and set cursor posiotion
        cycle = 0;
        
        //this "if" checks if mouse is sending 4th data byte
        if( device_id == 4 || device_id == 3)
           cycle = 3;

        ps2mouse_read(mouse_data+2);
        mouse_x += (mouse_data[1] - ((mouse_data[0] << 4) & 0x100));
        mouse_y += (mouse_data[2] - ((mouse_data[0] << 3) & 0x100));
        ps2mouse_set_cursor_position(mouse_x, mouse_y);

        break;
    case 3: 
        //case 3  deals with additional buttons (4th and 5th) and scroll parameters
        cycle = 0;
        ps2mouse_read(mouse_data+3);
        tmp = mouse_data[3];

        //z movement parameters vertical (up/down)
        if(0x0F & tmp == 0x0F)
            mouse_z_vertical-=1;
        if(0x01 & tmp == 0x01)
            mouse_z_vertical+=1;
        
        //z movement parameters horizontal (left/right)
        if( 0x02 & tmp == 0x02)
            mouse_z_horizontal+=1;
        if(0x0E & tmp == 0x0E)
            mouse_z_horizontal-=1;
        
        ps2mouse_set_scroll(mouse_z_vertical, mouse_z_horizontal);
        ps2mouse_set_additional_buttons(tmp);

        /*  DISPLAY FUNCTIONS   */

        x_display();
        y_display();
        //scroll_vertical_display();
        //scroll_horizontal_display();
        //middle_click_display();
        //right_click_display();
        left_click_display();
        //add1_click_display();
        //add2_click_display();
        double_click_display();


        break;
   }
    
    return true;
}

//function to check if we can send or receive data
BufferStatus ps2mouse_check_buffer_status(BufferType type)
{
    // for input mouse should return empty end for output mouse should return full
    uint32_t i = TIMEOUT;
    switch (type)
    {
    case Input:
        //in Input case we want to send command to mouse so the port have to be empty
        while (i)
        {
            if ((io_in_byte(PS2_CMD_PORT) & 0x02) == 0)
            {
                return Empty;
            }
            i--;
        }
        return Full;

    case Output:
        //in Output case we want to take data from mouse so port have to be full with some data
        while (i)
        {
            if ((io_in_byte(PS2_CMD_PORT) & 0x01) == 1)
            {
                return Full;
            }
            i--;
        }
        return Empty;
    };
}

//function to sending commands to mouse
bool ps2mouse_write(uint8_t byte)
{
	//check if mouse Input is free (able to send data) if it is not empty return false - there's no place to send data
    if (ps2mouse_check_buffer_status(Input) != Empty)
    {
        return false;
    }
	
	//sending a command to mouse to inform it that we want to send command
    io_out_byte(PS2_CMD_PORT, PS2_MOUSE_CMD_I_WANT_TO_SEND_COMMAND);

	//check againg if Input is empty
    if (ps2mouse_check_buffer_status(Input) != Empty)
    {
        return false;
    }
	
	//sending our stuff
    io_out_byte(PS2_DATA_PORT, byte);

    counter = PS2_MOUSE_TIMEOUT;
    //we have to wait for mouse to accept sent command
    while (counter-- >= 1)
    {
    	//mouse have to send sth
        if (!ps2mouse_read(&tmp))
            return false;
        //if mouse sent acknowledge (it accept command), loop is ended  
        if (tmp == PS2_MOUSE_CMD_ACK)
            break;
    }

    return true;
}

//function to get data from mouse and save it in byte
bool ps2mouse_read(uint8_t *byte)
{
	//mouse need place to save data - where byte not exists return false
    if (byte == NULL)
    {
        return false;
    }
    //If mouse Output is empty return false - on Output there's no data
    if (ps2mouse_check_buffer_status(Output) != Full)
    {
        return false;
    }
	//take the stuff that the mouse sent to us and put it into byte
    *byte = io_in_byte(PS2_DATA_PORT);

    return true;
}

//ps2 mouse scroll and additional buttons initialization
void ps2mouse_scroll_init(SequenceType type)
{
    //magic sequence to activate scroll
    uint8_t activ_scroll_sample_rates[3] = {0xC8, 0x64, 0x50}; //200 = C8(16), 100 = 64(16), 80 = 50(16)
    //magic sequence to activate additional buttons
    //NOTE: if addiional buttons are not enable mouse send 0xFF insted of 0xF when down scroll button is pressed
    uint8_t activ_buttons_sample_rates[3] = {0xC8, 0xC8, 0x50}; //200, 200, 50 
    uint8_t usd[3];

    //setting usd to chosen option
    for(int i=0; i<3; i++)
    {
        if(type == ScrollEnable)
            usd[i] = activ_scroll_sample_rates[i];
        if(type==AdditionalButonsEnable)
            usd[i] = activ_buttons_sample_rates[i];
    }
    
    //seqence - need to set 3 sample rates to activate scroll or 4th and 5th button
    for(int i=0; i<3; i++)
    {
        if (ps2mouse_check_buffer_status(Input) == Empty)
        {
            ps2mouse_write(PS2_MOUSE_CMD_SET_SAMPLE_RATE);
        }	
        if (ps2mouse_check_buffer_status(Input) == Empty)
        {
             ps2mouse_write(usd[i]);
        }
    }
}

//gets mouse x and y
void ps2mouse_get_cursor_position(int32_t* x, int32_t* y)
{
    *x = litte_squeaker.mouse_x;
    *y = litte_squeaker.mouse_y;
}

//setting mouse x and y
void ps2mouse_set_cursor_position(int32_t x, int32_t y)
{
    litte_squeaker.mouse_x = x;
    litte_squeaker.mouse_y = y;
}

//setting standart buttons - left, rigth and middle, last click was left is need for double click
void ps2mouse_set_standard_buttons(uint8_t byte)
{
    litte_squeaker.left_click = (byte & 0x01);
    litte_squeaker.right_click = (byte & 0x02);
    litte_squeaker.middle_click = (byte & 0x04);

    if(byte & 0x01)
    {
        litte_squeaker.last_click_was_left = 1;
        litte_squeaker.last_click_time = timer_get_system_clock();
    }
        
    if(byte & 0x06)
        litte_squeaker.last_click_was_left = 0;
}

//setting additional buttons if mouse have ones
void ps2mouse_set_additional_buttons(uint8_t byte)
{
    litte_squeaker.additional_button_1 = (byte & 0x20);
    litte_squeaker.additional_button_2 = (byte & 0x40);

    if(byte & 0x60)
        litte_squeaker.last_click_was_left = 0;
}

//setting mouse scroll parameters
void ps2mouse_set_scroll(int32_t mouse_z_v, int32_t mouse_z_h)
{
    litte_squeaker.mouse_z_horizontal = mouse_z_h;
    litte_squeaker.mouse_z_vertical = mouse_z_v;
}

//gets mouse middle button
bool ps2mouse_get_middle()
{
    return litte_squeaker.middle_click;
}

//gets mouse right button
bool ps2mouse_get_right()
{
    return litte_squeaker.right_click;
}

//gets mouse left button
bool ps2mouse_get_left()
{
    return litte_squeaker.left_click;
}

//gets mouse 4th button
bool ps2mouse_get_add_b4()
{
    return litte_squeaker.additional_button_1;
}

//gets mouse 5th button
bool ps2mouse_get_add_b5()
{
    return litte_squeaker.additional_button_2;
}

//gets double click
bool ps2mouse_get_double_click()
{
    return litte_squeaker.double_click;
}


//gets z movement vertical
uint16_t ps2mouse_get_scroll_v()
{
    return litte_squeaker.mouse_z_vertical;
}
//gets z movement horizontal
uint16_t ps2mouse_get_scroll_h()
{
    return litte_squeaker.mouse_z_horizontal;
}

//Display functions - to check if mouse  is working correctly
//in code are marked places where this functions should be, just delete "//" id that places 

void double_click_display()
{
    if(litte_squeaker.double_click == 1)
        logger_log_info("Double click");
}

void right_click_display()
{
    if(litte_squeaker.right_click == 1)
        logger_log_info("Right click");
}

void left_click_display()
{
    if(litte_squeaker.left_click == 1)
        logger_log_info("Left click");
}

void middle_click_display()
{
    if(litte_squeaker.middle_click == 1)
        logger_log_info("Middle click");
}

void add1_click_display()
{
    if(litte_squeaker.additional_button_1 == 1)
        logger_log_info("Additional button 1");
}

void add2_click_display()
{
    if(litte_squeaker.additional_button_2 == 1)
        logger_log_info("Additional button 2");
}

void scroll_vertical_display()
{
    uint32_t z = litte_squeaker.mouse_z_vertical;
    char str[100] = "ZV:";
    itoa(z, str+3, 10);
    logger_log_info(str);
}

void scroll_horizontal_display()
{
    uint32_t z = litte_squeaker.mouse_z_horizontal;
    char str[100] = "ZH:";
    itoa(z, str+3, 10);
    logger_log_info(str);
}

void x_display()
{
    uint32_t x = litte_squeaker.mouse_x;
    char str[100] = "X:";
    itoa(x, str+2, 10);
    //logger_log_info(str);
}

void y_display()
{
    uint32_t y = litte_squeaker.mouse_y;
    char str[100] = "Y:";
    itoa(y, str+2, 10);
    //logger_log_info(str);
}