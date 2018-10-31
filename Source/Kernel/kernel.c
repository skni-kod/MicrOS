int kmain()
{
    volatile char* video = (volatile char*)0xB8000;
    
    *video = 'S';
    *(video + 2) = 'K';
    *(video + 4) = 'N';
    *(video + 6) = 'I';
    *(video + 8) = ' ';

    return 0;
}