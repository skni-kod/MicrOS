#include "../locale.h"

lconv *localeconv()
{
    return locale_state;
}