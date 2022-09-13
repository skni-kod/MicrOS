#include "game.h"

int board_width = 70;
int board_height = 19;

const int console_width = 80;
const int console_height = 25;

const int new_food_count = 10;
const int generate_food_edge = 7;
const float acceleration_ratio = 0.03f;

int board_initial_x;
int board_initial_y;

char **board;

game_state state;
vector snake;
direction current_dir;
direction future_dir;

float current_acceleration;
int food_count;
int eaten_food;
int health;

bool redraw_board;
bool game_exit_to_menu;
unsigned int last_update;

void game_init()
{
    game_generate();
    game_generate_snake();
    
    current_dir = dir_right;
    future_dir = dir_right;
    current_acceleration = 0;
    food_count = 0;
    eaten_food = 0;
    health = 3;
    game_exit_to_menu = false; 
    
    last_update = micros_timer_get_system_clock();
    state = game_state_running;
    
    timer_reset();
}

void game_input()
{
    while (micros_keyboard_is_key_pressed())
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_get_pressed_key(&pressed_key);

        switch (pressed_key.scancode)
        {
            case key_keypad_8: future_dir = current_dir != dir_down ? dir_up : current_dir; break; 
            case key_keypad_2: future_dir = current_dir != dir_up ? dir_down : current_dir; break; 
            case key_keypad_4: future_dir = current_dir != dir_right ? dir_left : current_dir; break; 
            case key_keypad_6: future_dir = current_dir != dir_left ? dir_right : current_dir; break;
            case key_esc: game_exit_to_menu = true; break;
        }
    }
}

scene_type game_logic()
{
    if(game_exit_to_menu)
    {
        return scene_type_main_menu;
    }
    
    float time_edge = 0;
    switch (future_dir)
    {
        case dir_up:
        case dir_down:
        {
            time_edge = 250;
            break;
        }
        
        case dir_right:
        case dir_left:
        {
            time_edge = 200;
            break;
        }
    }
    
    time_edge -= current_acceleration;
    if(time_edge < 0)
    {
        time_edge = 0;
    }
    
    if(micros_timer_get_system_clock() - last_update >= time_edge)
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
            case EMPTY_FIELD_SYMBOL:
            {
                point *tail = snake.data[snake.count - 1];
                vector_remove(&snake, snake.count - 1);
                board[tail->x][tail->y] = ' ';
                break;
            }
            
            case FOOD_SYMBOL:
            {
                food_count--;
                eaten_food++;
                break;
            }
            
            case WALL_SYMBOL:
            case SNAKE_SYMBOL:
            {
                state = game_state_loss;
                break;
            }
        }
        
        vector_insert(&snake, new_head, 0);
        board[new_head->x][new_head->y] = SNAKE_SYMBOL;
        
        redraw_board = true;
    }
    
    if(state == game_state_loss)
    {
        game_animate_death();
        
        if(health > 1)
        {
            game_reset_snake();
            
            health--;
            current_acceleration /= 2;
            future_dir = dir_right;
            current_dir = dir_right;
            
            state = game_state_running;
            
            return scene_type_none;
        }
        else
        {
            if(eaten_food >= highscores_get_minimal_score_to_save())
            {
                new_highscore_message_draw_and_wait(eaten_food);
                return scene_type_highscores;
            }
            else
            {
                loss_message_draw_and_wait(eaten_food);
                return scene_type_main_menu;
            }
        }
    }
    
    if(food_count < generate_food_edge)
    {
        game_generate_food();
    }
    
    current_acceleration += acceleration_ratio;
    return scene_type_none;
}

void game_draw()
{
    if(redraw_board)
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
                    case EMPTY_FIELD_SYMBOL:
                    {
                        micros_console_set_background_color(micros_console_color_dark_gray);
                        break;
                    }
                    
                    case WALL_SYMBOL:
                    {
                        micros_console_set_background_color(micros_console_color_light_gray);
                        micros_console_set_foreground_color(micros_console_color_dark_gray);
                        break;
                    }
                    
                    case FOOD_SYMBOL:
                    {
                        micros_console_set_background_color(micros_console_color_light_green);
                        micros_console_set_foreground_color(micros_console_color_green);
                        break;
                    }

                    case SNAKE_SYMBOL:
                    {
                        micros_console_set_background_color(micros_console_color_cyan);
                        micros_console_set_foreground_color(micros_console_color_red);
                        break;
                    }
                }

                micros_console_print_char(board[x][y]);
            }
        }
    
        interface_display(1, eaten_food, health);
        timer_display(37, 23);
        
        redraw_board = false;
    }
}

void game_delete()
{
    vector_clear(&snake);
    
    for (int i = 0; i < board_width; i++)
    {
        free(board[i]);
    }
    free(board);
    
    board = 0;
}

int game_get_eaten_food()
{
    return eaten_food;
}

void game_generate()
{
    if(board == 0)
    {
        board = malloc(sizeof(char *) * board_width);
        for (int i = 0; i < board_width; i++)
        {
            board[i] = malloc(sizeof(char) * board_height);
        }
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

void game_generate_snake()
{
    vector_init(&snake);

    point *head = malloc(sizeof(point));
    head->x = console_width / 2;
    head->y = console_height / 2;

    vector_add(&snake, head);
    board[head->x][head->y] = 'S';
}

void game_generate_food()
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

void game_animate_death()
{
    for(int i = 0; i < 10; i++)
    {
        game_set_snake_visibility(i % 2);
        micros_process_current_process_sleep(300);
    }
}

void game_set_snake_visibility(bool visible)
{
    micros_console_position cursor_position;
    for(unsigned int i = 0; i < snake.count; i++)
    {
        point *snake_part = snake.data[i];
        
        cursor_position.x = snake_part->x + board_initial_x;
        cursor_position.y = snake_part->y + board_initial_y;
        micros_console_set_cursor_position(&cursor_position);
                
        if(visible)
        {
            micros_console_set_background_color(micros_console_color_cyan);
            micros_console_set_foreground_color(micros_console_color_red);
            micros_console_print_char(SNAKE_SYMBOL);
        }
        else
        {
            micros_console_set_background_color(micros_console_color_dark_gray);
            micros_console_print_char(EMPTY_FIELD_SYMBOL);
        }
    }
}

void game_reset_snake()
{
    vector_clear(&snake);
    food_count = 0;
    
    game_generate();
    game_generate_snake();
}