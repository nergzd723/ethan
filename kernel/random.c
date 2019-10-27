//pseudorandom code

//seed needs to be seeded 0_0
static unsigned long int next = 1;

//returns randint
int rand( void ) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}
 
//seed seeding
void srand( unsigned int seed )
{
    next = seed;
}