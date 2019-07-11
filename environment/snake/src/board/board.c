#include "board.h"

int board_width;
int board_height;

int board_initial_x;
int board_initial_y;

char **board;
vector snake;
direction dir;

unsigned int last_update;

const int console_width = 80;
const int console_height = 25;

void board_init(int width, int height)
{
    board_width = width;
    board_height = height;
    
    board = malloc(sizeof(char *) * width);
    for(int i = 0; i < width; i++)
    {
        board[i] = malloc(sizeof(char) * height);
    }

    board_initial_x = (console_width / 2) - (board_width / 2);
    board_initial_y = (console_height / 2) - (board_height / 2);

    for (int x = 0; x < board_width; x++)
    {
        for (int y = 0; y < board_height; y++)
        {
            if (x == 0 || x == board_width - 1 || y == 0 || y == board_height - 1)
            {
                board[x][y] = '#';
            }
            else
            {
                board[x][y] = ' ';
            }
        }
    }
    
    vector_init(&snake);

    point *head = malloc(sizeof(point));
    head->x = console_width / 2;
    head->y = console_height / 2;
    
    vector_add(&snake, head);
    board[head->x][head->y] = 'S';
    
    last_update = micros_timer_get_system_clock();
}

bool board_logic()
{
    if(micros_keyboard_is_key_pressed())
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_get_pressed_key(&pressed_key);
        
        switch(pressed_key.scancode)
        {
            case key_w:
            {
                dir = dir_up;
                break;
            }
            
            case key_s:
            {
                dir = dir_down;
                break;
            }
            
            case key_a:
            {
                dir = dir_left;
                break;
            }
            
            case key_d:
            {
                dir = dir_right;
                break;
            }
        }
    }
    
    if(micros_timer_get_system_clock() - last_update > 100)
    {
        last_update = micros_timer_get_system_clock();
        
        point *old_head = snake.data[0];
        point *new_head = malloc(sizeof(point));
        new_head->x = old_head->x;
        new_head->y = old_head->y;

        switch(dir)
        {
            case dir_up:
            {
                new_head->y--;
                break;
            }
            
            case dir_down:
            {
                new_head->y++;
                break;
            }
            
            case dir_right:
            {
                new_head->x++;
                break;
            }
            
            case dir_left:
            {
                new_head->x--;
                break;
            }
        }

        if (board[new_head->x][new_head->y] != '*')
        {
            point *tail = snake.data[snake.count - 1];
            
            vector_remove(&snake, snake.count - 1);
            board[tail->x][tail->y] = ' ';
        }
        else
        {
            __asm__("nop");
        }
        
        
        vector_insert(&snake, new_head, 0);
        board[new_head->x][new_head->y] = 'S';
    }
    
    return true;
}

void board_draw()
{
    micros_console_position cursor_position;
    
    for(int x = 0; x < board_width; x++)
    {
        for(int y = 0; y < board_height; y++)
        {
            cursor_position.x = x + board_initial_x;
            cursor_position.y = y + board_initial_y;
            micros_console_set_cursor_position(&cursor_position);
            
            switch(board[x][y])
            {
                case ' ':
                {
                    micros_console_set_background_color(micros_console_color_dark_gray);
                    break;
                }
                
                case '#':
                {
                    micros_console_set_background_color(micros_console_color_light_gray);
                    micros_console_set_foreground_color(micros_console_color_dark_gray);
                    break;
                }
                
                case '*':
                {
                    micros_console_set_background_color(micros_console_color_light_green);
                    micros_console_set_foreground_color(micros_console_color_green);
                    break;
                }

                case 'S':
                {
                    micros_console_set_background_color(micros_console_color_cyan);
                    micros_console_set_foreground_color(micros_console_color_red);
                    break;
                }
            }

            micros_console_print_char(board[x][y]);
        }
    }
}

void board_generate_food(int count)
{
    for(int i = 0; i < count; i++)
    {
        int x = rand() % (board_width - 2) + 1;
        int y = rand() % (board_height - 2) + 1;
        
        board[x][y] = '*';
    }
}