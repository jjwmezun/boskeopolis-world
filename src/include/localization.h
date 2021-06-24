#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include "assoc_array.h"

typedef struct
{
    AssocArray charmap;
}
Localization;

void localization_init();
void localization_close();
Localization * localization_get_current();

#endif // LOCALIZATION_H