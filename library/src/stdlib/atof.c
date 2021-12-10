#include "../stdlib.h"
#include "../math.h"

double atof(const char *string)
{
    int i,j,flag;
    float val;
    char c;
    i=0;
    j=0;
    val=0;
    flag=0;
    while ((c = *(string+i))!='\0'){
//      if ((c<'0')||(c>'9')) return 0;
        if (c!='.'){
            val =(val*10)+(c-'0');
            if (flag == 1){
                --j;
            }
        }
        if (c=='.'){ if (flag == 1) return 0; flag=1;}
        ++i;
    }
    val = val*pow(10,j);
    return val;
}