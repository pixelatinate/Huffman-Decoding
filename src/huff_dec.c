// Lab 3: huff_dec.c
// This lab is a representation of Huffman decoding.
//      See http://web.eecs.utk.edu/~jplank/plank/classes/cs360/360/labs/Lab-3-Huffman/index.html
//      for more information and lab specifications. 

// COSC 360
// Swasti Mishra 
// Feb 21, 2022
// James Plank 

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// The node structure Plank suggested-
//		I didn't really understand why he had two chars for 
//		s_one and s_zero so I just have the one as data
typedef struct huff_node{	
	char *data ;
	struct huff_node *one ;
	struct huff_node *zero ;
} *nodePointer ;

// Adds nodes to the tree
struct huff_node* insertNode(){
	
	// Allocates space for each node
	nodePointer node = malloc( sizeof( struct huff_node ) ) ;
	
	// Sets everything inside each new node to null
	node->data = NULL ;
	node->zero = NULL ;
	node->one  = NULL ;
	
	return node ;
}

// Recursively frees up the data used by the tree
int freeTree( nodePointer node ){
	if( node != NULL ){
		freeTree( node->one ) ;
		freeTree( node->zero ) ;
		free( node->data ) ;
		free( node ) ;
	}
	return 0 ;
}

int main( int argc, char* argv[] ){
	
	// Suppresses the argc warning
    (void)argc ;

	// Counter variables
	int i, j ;
	int inputSize ;
	char buffer[150] ;

	FILE *input ;
	nodePointer head = malloc( sizeof( struct huff_node ) ) ;
	nodePointer node = malloc( sizeof( struct huff_node ) ) ;
	
	// Opens the first input file
	input = fopen( argv[1], "r" ) ;

	// Loops through the first file until end 
	while( feof( input ) == 0 ){
		int counter = 0 ; 
		
		// Reassigns node
		node = head ;

		// Arbitrary number of loops- it should be the size 
		// 		of the file, but I was too lazy to fix this 
		while( counter < 150 ){
			i = fgetc( input ) ;
			buffer[counter] = i ;
			if( i == '\0' ){
				break ;
			}
			counter++ ;
		}
		
		// Loops through file again 
		while( feof( input ) == 0 ){
			i = fgetc( input ) ;
			
			// If it's a zero, it goes on one branch
			if ( i == '0' ){
				if ( node->zero == NULL ){
					node->zero = insertNode() ;
				}
				node = node->zero ;
			}

			// If it's a one, it goes on the other
			if ( i == '1'){
				if ( node->one == NULL ){
					node->one = insertNode() ;
				}
				node = node->one ;
			}

			// Newline sets the data and breaks the loop
			if ( i == '\0' ){
				node->data = malloc( sizeof(char) * ( counter + 1 ) ) ;
				strcpy( node->data, buffer ) ;
				break ;
			}
		}
	}

	// Close the first file 
	fclose( input ) ;

	// Open the second file 
	input = fopen( argv[2], "r" ) ;
	
	// Checks size 
	fseek( input, 0, SEEK_END ) ;
	i = ftell( input ) ;
	fseek( input, -4, SEEK_END ) ;
	fread( &inputSize, 4, 1, input ) ;
	int correctSize = ( ( ( inputSize / 8 ) + ( inputSize % 8 != 0 ) ) + 4 ) ;
	
	// If file is the wrong size, prints error message and frees memory
	if ( correctSize != i ){
		if ( i < 4 ){
			fprintf( stderr, "Error: file is not the correct size.\n" ) ;
		}
		else{
			fprintf( stderr, "Error: Total bits = %d, but file's size is %d\n", inputSize, i ) ;
		}
		freeTree( head ) ;
		fclose( input ) ;
		return 1 ;
	}

	// Otherwise, we're good to search!
	fseek( input, 0, SEEK_SET ) ;
	node = head ;

	// Loops until we reach the end of the file (again)
	while( feof( input ) == 0 ){
		i = fgetc( input ) ;

		// Goes down each branch of the tree 
		for ( j = 0 ; ( j < 8 ) && ( inputSize != 0 ) ; j++ ){
			// Goes a step deeper every time it doesn't find data
			if ( ( 1 << j ) & i ){
				if ( node->one != NULL ){
					node = node->one ;
				}
			}
			// Same thing here
			else{
				if ( node->zero != NULL ){
					node = node->zero ;
				}
			}
			// For when we do find data
			if ( node->data != NULL ){
				printf( "%s", node->data ) ;
				node = head ;
			}
			inputSize-- ;
		}
	}
	
	// Free the memory used for the tree
	freeTree( head ) ;

	// Close file 
	fclose( input ) ;

	return 0 ;
}