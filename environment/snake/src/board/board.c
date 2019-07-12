#include "board.h"

int board_width;
int board_height;

int board_initial_x;
int board_initial_y;

char **board;
board_state state;
vector snake;
direction current_dir;
direction future_dir;
float current_acceleration;
int food_count;
int eaten_food;

int new_food_count;
int generate_food_edge;
float acceleration_ratio;

unsigned int last_update;

const int console_width = 80;
const int console_height = 25;

void board_init(int _width, int _height, int _new_food_count, int _generate_food_edge, float _acceleration_ratio)
{
    board_width = _width;
    board_height = _height;
    new_food_count = _new_food_count;
    generate_food_edge = _generate_food_edge;
    acceleration_ratio = _acceleration_ratio;
    
    board_generate();
    board_generate_snake();
        
    last_update = micros_timer_get_system_clock();
    state = board_state_running;
}

bool board_logic()
{
    if(micros_timer_get_system_clock() - last_update >= 200 - current_acceleration)
    {
        last_update = micros_timer_get_system_clock();
        
        point *old_head = snake.data[0];
        point *new_head = malloc(sizeof(point));
        new_head->x = old_head->x;
        new_head->y = old_head->y;

        switch(future_dir)
        {
            case dir_up: new_head->y--; break;
            case dir_down: new_head->y++; break;
            case dir_right: new_head->x++; break;
            case dir_left: new_head->x--; break;
        }
        current_dir = future_dir;

        switch (board[new_head->x][new_head->y])
        {
            case ' ':
            {
                point *tail = snake.data[snake.count - 1];
                vector_remove(&snake, snake.count - 1);
                board[tail->x][tail->y] = ' ';
                break;
            }
            
            case '*':
            {
                food_count--;
                eaten_food++;
                break;
            }
            
            case '#':
            case 'S':
            {
                state = board_state_loss;
                break;
            }
        }
        
        vector_insert(&snake, new_head, 0);
        board[new_head->x][new_head->y] = 'S';
        
        return true;
    }
    
    if(food_count < generate_food_edge)
    {
        board_generate_food();
    }
    
    current_acceleration += acceleration_ratio;
    return false;
}

void board_input()
{
    while (micros_keyboard_is_key_pressed())
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_get_pressed_key(&pressed_key);

        switch (pressed_key.scancode)
        {
            case key_w: future_dir = current_dir != dir_down ? dir_up : current_dir; break; 
            case key_s: future_dir = current_dir != dir_up ? dir_down : current_dir; break; 
            case key_a: future_dir = current_dir != dir_right ? dir_left : current_dir; break; 
            case key_d: future_dir = current_dir != dir_left ? dir_right : current_dir; break; 
        }
    }
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

board_state board_get_state()
{
    return state;
}

int board_get_eaten_food()
{
    return eaten_food;
}

void board_generate()
{
    board = malloc(sizeof(char *) * board_width);
    for (int i = 0; i < board_width; i++)
    {
        board[i] = malloc(sizeof(char) * board_height);
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
}

void board_generate_snake()
{
    vector_init(&snake);

    point *head = malloc(sizeof(point));
    head->x = console_width / 2;
    head->y = console_height / 2;

    vector_add(&snake, head);
    board[head->x][head->y] = 'S';
}

void board_generate_food()
{
    for(int i = 0; i < new_food_count; i++)
    {
        int x = rand() % (board_width - 2) + 1;
        int y = rand() % (board_height - 2) + 1;
        
        if(board[x][y] == ' ')
        {
            board[x][y] = '*';
            food_count++;
        }
    }
}