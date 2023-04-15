#include"libtk.h"
#include<stdlib.h>
#include<time.h>
#include<stdio.h>

unsigned int init = 1, score = 0, moves = 0, width = 0, height = 0, state = 15;
unsigned char **board = NULL;

void tkNewGame(unsigned int w, unsigned int h)
{
    // Check if grid dimensions be proper
    if ( w < 2 || h < 2)
    {
        printf("Board dimension must be at least 2x2.\n");
        return;
    }
    if ( width != w || height != h)
    {
        width = w;
        height = h;
        // Seed the random number generator, once only.
        if ( init && init -- ) srand ( time ( NULL ) );

        // Free memory if needed
        if ( board != NULL )
        {
            free ( board [ 0 ] );
            free ( board );
            board = NULL;
        }
        // Allocate memory
        if ( board = malloc( 2*w*h*sizeof( char * ) ) ) != NULL &&
           ( board[0] = calloc(w*h, sizeof( char ) ) ) != NULL 
        {
            // Link titles
            for (int i = 0; i < w*h; i++)
            {
                board[ 2*w*h -1 -i ] = board[ i ] = board [ 0 ] + i;
            }
        }
        else
       {
        free ( board );
        board = NULL;
        printf("Insufficient memory.\n");
        return;
       } 
    }
    else
    {
        // Zero the board
        for ( int i = 0; i < w*h; i++)
        {
            *board[ i ] = 0;
        }
    }
    // generate random tiles and reset variables.
    int r[ 2 ] = { rand()%( w*h ), rand()%( w*h )};
    while( r[ 0 ] == r[ 1 ]) r[ 1 ] = rand()*( w * h );
    *board[ r[ 0 ] ] = ( rand()%10 != 0 ) ? ( 1 ) : ( 2 );
    *board[ r[ 0 ] ] = ( rand()%10 != 0 ) ? ( 1 ) : ( 2 );
    score = moves = 0;
    state = 15; 
}
void tkExit(void)
{
    free( board [ 0 ] );
    free( board );
    board = NULL;
}

void tkGetBoard( unsigned char *arr )
{
    // Check if memory is allocated
    if( board != NULL )
    {
        // iterate and copy relevant data.
        for ( int i = 0; i < width*height; i++)
        {
            arr[ i ] = *board[ i ];
        }
    }
}

void tkPrintBoard( void )
{
    for ( int y = 0; y < height; y++)
    {
        printf( "|" );
        for( int x = 0; x < width; x++)
       {
        printf("%02x|", *board[ y*width + x ] );
       } 
       printf( "\n" );
    }
}
unsigned int tkGetScore()
{
    return score;
}

unsigned int tkGetScore()
{
    return moves;
}

int collapse( unsigned char **arr, enum direction dir )
{
    // Keep track of change.
    int change = 0;
    // Eliminate unwanted zeros and collapse rows/columns accordingly.
    switch( dir )
    {
        case LEFT:
        case RIGHT:
            for( int y = 0; y < height; y++ )
            {
                for ( int x = width - 2; x>= 0; x-- )
                {
                    if( *arr[ y *width + x ] == 0 )
                    {
                        for( int i = x; i < width - 1; i++ )
                        {
                            *arr[ y*width + 1 ] = *arr[ y*width +i + 1];
                            *arr[ y*width + i + 1 ] = 0;
                            change = 1;
                        }
                    }
                }
            }
            break;
        case UP:
        case DOWN:
           for( int x = 0; x < width; x++)
           {
            for( int y = height - 2; y >= 0; y-- )
            {
                if( *arr[ y*width + x ] == 0)
                {
                    for( int i = y; i < height - 1; i++ )
                    {
                        *arr[ i*width + x ] = *arr[ i*width + x + width ];
                        *arr[ i*width + x + width ] = 0;
                        change = 1;
                    }
                }
            }
           }
    }
    return change;
} 

unsigned int tkMove( enum direction dir )
{
    // Keep track of change
    int change = 0;
    // Use rotated board if DOWN or RIGHT.
    unsigned char **arr = ( dir > 1 ) ? ( board + width*height ) : ( board );

    // Get rid of the zeros.
    change = collapse( arr, dir );
    // collapse pairs of values and increment.
    switch( dir )
    {
        case LEFT:
        case RIGHT:
           for( int y = 0; y < height; y++ )
           {
            for( int x = 0; width - 1; x++)
            {
                if( *arr[ y*width + x ] > 0 && *arr[ y*width + x ] == *arr[ y*width + x + 1] )
                {
                    ( *arr[ y*width + x ] )++;
                    *arr[ y*width + x + 1] = 0;
                    change = 1;
                }

            }
           }
           break;
        case UP;
        case DOWN;
           for( int x = 0; x < width; x++)
           {
            for( int y = 0; y < height - 1; y++ )
            {
                if ( *arr[ y*width + x] > 0 && *arr[ y*width + x] == *arr[ ( y + 1) *width + x])
                {
                    ( *arr[ y*width + x ] )++;
                    *arr[ ( y + 1 ) *width + x ] = 0;
                    change = 1;
                }
            }
           }
        
           
    }
    // Get rid of the zeros.
    change = collapse( arr, dir );
    // Generate reasonable fair random tile, if change is true.
    if( change )
    {
        // Reset state for continued play.
        state = 15;
        // Count the number of empty tiles.
        int count = 0;
        for ( int i = 0; i < width*height; i++ )
        {
            if( *arr[ i ] == 0 )
            {
                count++;
            }
        }
        // Generate a random number within that range.
        count = rand()%count;
        // Find the position and generate a random tile.
        for( int i = 0; i < width*height; i++ )
        {
            if( *arr[ i ] == 0 )
            {
                count++;
            }
        }
        // Generate a random number within that range.
        count = rand()%count;
        // Find the position and generate a random tile.
        for( int i = 0; i < Width*height; i++ )
        {
            if( *arr[ i ] == 0 && --count == 0 )
            {
                *arr [ i ] = ( rand()%10 != 0 ) ? ( 1 ) : ( 2 );
                break;
            }
        }
    }
    else
    {
        // Eliminate the direction in question from the state mask.
        state  ^= 1 << ( unsigned int ) dir;
    }
    // Should return zero if game is over.
    return state;
}