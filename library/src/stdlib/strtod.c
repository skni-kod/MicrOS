#include "../stdlib.h"
#include "../ctype.h"
#include "../math.h"

// convert string to double variable
double _atod(char **str)
{
    double total_part = 0;
    double decimal_part = 0;
    float decimal_div = 0.1;
    short sign = 1;

    while (isspace(**str))
        (*str)++;

    if (**str == '-')
        sign = -1;

    if (**str == '-' || **str == '+')
        (*str)++;

    while (**str >= '0' && **str <= '9')
    {
        total_part = total_part * 10 + (**str) - '0';
        ++(*str);
    }

    // decimal part
    if (**str == '.')
    {
        ++(*str);
        while (**str >= '0' && **str <= '9')
        {
            decimal_part = decimal_part + (**str - '0') * decimal_div;
            decimal_div *= 0.1;

            ++(*str);
        }
    }

    return (total_part + decimal_part) * sign;
}

double strtod(const char *str, char **endptr)
{
    /* 
    @INCOMPLETE
    -> Handle hex numbers
    -> set errno error in case of overflow (how to detect it..?)
    -> return 0.0 if cannot convert 
    */
    double base_number = _atod((char **)&str);
    double exp_number = 0;

    // scientific notation
    if (*str == 'e' || *str == 'E')
    {
        ++str;
        exp_number = _atod((char **)&str);

        base_number *= pow(10, exp_number);
    }

    if (endptr != NULL)
        *endptr = (char *)str;

    return base_number;
}