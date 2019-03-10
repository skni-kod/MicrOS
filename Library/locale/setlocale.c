#include "../locale.h"

char *setlocale(int category, const char *locale)
{
    if (locale_state == NULL)
    {
        locale_state = calloc(1, sizeof(lconv));
    }

    // TODO: replace it with strcmp
    if (locale[0] == 'C')
    {
        __locale_set_neutral_locale();
    }

    return 0;
}