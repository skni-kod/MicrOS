// int printString(char* string)
// { 
//  int i = 0;
//  while (*(string + i) != '\0')
//    {
//     char al = *(string + i);
//     char ah = 0xe;
//     int ax = ah * 256 + al;
//     interrupt(0x10,ax,0,0,0);
//     i++;
//    }
//  return i;
// }