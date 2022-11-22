// test function
void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = colour;
    }
}

void kernel_main() 
{
	write_string(5, "Welcome to AlmorinaOS!");
}
