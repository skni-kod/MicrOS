#include "board.h"

int board_width;
int board_height;

int board_initial_x;
int board_initial_y;

char **board;

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
}

bool board_logic()
{
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