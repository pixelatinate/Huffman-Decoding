// Lab 3: huff_dec.c
// This lab is a representation of Huffman decoding.
//      See http://web.eecs.utk.edu/~jplank/plank/classes/cs360/360/labs/Lab-3-Huffman/index.html
//      for more information and lab specifications. 

// COSC 360
// Swasti Mishra 
// Feb 21, 2022
// James Plank 

# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// ??
# include "jrb.h"

struct huff_node{
  char *value ;
  struct huff_node *p_left ;
  struct huff_node *p_right ;
} ;

typedef int ( *Compare )( const char *, const char * ) ;

void insertNode( char *key, struct huff_node** leaf, Compare cmp ){
    int res;
    if( *leaf == NULL ){
        *leaf = ( struct huff_node* ) malloc( sizeof( struct huff_node ) ) ;
        (*leaf)->value = malloc( strlen (key) + 1 ) ;
        strcpy ( ( *leaf )->value, key ) ;
        ( *leaf )->p_left = NULL ;
        ( *leaf )->p_right = NULL ;
    } 
    else{
        res = cmp ( key, (*leaf)->value ) ;
        if( res < 0 ){
            insertNode( key, &(*leaf)->p_left, cmp ) ;
        }
        else if( res > 0){
            insertNode( key, &(*leaf)->p_right, cmp ) ;
        }
        
    }
}

int main( int argc, char* argv[] ){

    // Suppresses the argc warning
    (void)argc ;

    // Counter Variables
    int i, j ;
    int c, counter ; 
    unsigned int currentBit = 0 ;
    int codeSize, inputSize ;
    char *buff = malloc( sizeof(char) ) ;
    
    // ??
    JRB t ;

    FILE *codeFile, *inputFile ;
    unsigned int *n = malloc( sizeof(unsigned int) ) ;
    
    // ??
    // struct huff_node* t = newNode( head ) ;
    t = make_jrb() ;
    
    codeFile = fopen( argv[1], "r" ) ;
    if ( (unsigned char *) codeFile < (unsigned char *) NULL ){
        perror( argv[1] ) ;
    }

    while( feof(codeFile) == 0 ){
        
        char* key   = malloc( sizeof(char)*256 ) ;
        char* value = malloc( sizeof(char)*256 ) ;
        
        fread( buff, sizeof(char), 1, codeFile ) ;
        while( buff[0] != '\0' ){
            if ( i != 0 ){
                fread( buff, sizeof(char), 1, codeFile ) ;
            }
            c = buff[0] ;
            value[i] = c ;
            i++ ;
        }
        i = 0 ;

        fread( buff, sizeof(char), 1, codeFile ) ;
        while( buff[0] != '\0' ){
            if (i != 0){
                fread( buff, sizeof(char), 1, codeFile ) ;
            }
            c = buff[0] ;
            key[i] = c ;
            i++ ;
        }
        i = 0 ;

        // ??
        if( feof(codeFile) == 0 ){
            jrb_insert_str( t, key, new_jval_s(value) ) ;
            //insertNode( t, key, (Compare)value ) ;
        }
    }
    fseek( codeFile, -4, SEEK_END ) ;
    fread( n, sizeof(unsigned int), 1, codeFile ) ;

    fseek( codeFile, 0, SEEK_END ) ;
    codeSize = ftell( codeFile ) ;

    fseek( codeFile, 0, SEEK_SET ) ; 
    fclose( codeFile ) ;

    inputFile = fopen( argv[2], "r" ) ;
    if ( (unsigned char *) inputFile < (unsigned char *) NULL ){
        perror( argv[2] ) ;
    }

    fseek( inputFile, -4, SEEK_END ) ;
    fread( n, sizeof(unsigned int), 1, inputFile ) ;

    fseek( inputFile, 0, SEEK_END ) ;
    inputSize = ftell( inputFile ) ;

    fseek( inputFile, 0, SEEK_SET ) ;

    int correctSize = ( ( *n / 8 ) + ( *n % 8 != 0 ) ) + 4 ;

    if( inputSize != correctSize ){
        if( inputSize < 4 ){
            fprintf( stderr, "Error: file is not the correct size.\n" ) ;
        }
        if( inputSize >= 4 ){
            fprintf( stderr, "Error: Total bits = %u, but file's size is %i\n", (*n), inputSize ) ;
        }
        return 1 ;
    }
    

    buff = realloc( buff, sizeof(char)*(*n) ) ;
    char stream[ (*n) ] ;

    fread( buff, 1, *n, inputFile ) ;
    for( i = 0; i < (*n) / 8.0; i++ ){
        for( j = 0; j < 8; j++ ){
           currentBit++ ;
            if( buff[i] & 1 ){
                stream[counter] = '1' ;
            } 
            else{
                stream[counter] = '0' ;
            }
            counter++ ;
            buff[i] = buff[i] >> 1 ;
            stream[counter] = '\0' ;

            // ??
            if( jrb_find_str( t, stream ) != NULL ){
                printf( "%s", jrb_find_str( t, stream )->val.s ) ;
                stream[0] = '\0' ;
                counter = 0 ;
            }
            if( currentBit >= *n ){
                fclose( inputFile ) ;
                return 0 ;
            }
        }
    }

    fclose( inputFile ) ;
    return 0 ;
}