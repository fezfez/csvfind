#include <stdio.h>
#include <string.h>
#include <ctype.h>

int trim( char * text, char *trimmed ) {
    strcpy( trimmed, text );
    int newline_pos = strcspn( trimmed, "\n" );
    if( trimmed[newline_pos] == '\n' ) {
        trimmed[newline_pos] = '\0';
    }

    return newline_pos;
}

char * to_lowercase( char *text ) {
    int len = strlen( text );

    for( int i = 0; i < len; i++ ) {
        text[i] = tolower( text[i] );
    }

    return text;
}

int main( int argc, char *argv[] ) {
    if( argc < 3 ) {
        printf( "Usage: %s INPUT_FILE SEARCH_TERM [HEADER_NAME]\n", argv[0] );
        return 1;
    }

    char *input_file = argv[1];
    char *search_term = argv[2];
    char *header_name;

    int search_header = argc >= 4;
    if( search_header ) {
        header_name = argv[3];
    }

    int buffer_length = 1024;
    char buffer[buffer_length];
    FILE *fp = fopen( input_file, "r" );

    if( fp == NULL ) {
        printf( "Error reading file %s", input_file );
        return 1;
    }

    char header_buffer[50][100];

    int line_number = 0;
    int print_line = 0;
    int col_number = 0;
    while( fgets( buffer, buffer_length, fp ) != NULL ) {
        print_line = 0;

        char line[buffer_length];
        strcpy( line, buffer );

        col_number = 0;

        char *token = strtok( buffer, "," );
        while( token != NULL ) {
            if( line_number == 0 ) {
                char trimmed[256];
                trim( token, trimmed );
                strcpy( header_buffer[col_number], trimmed );
            };

            char *current_header = header_buffer[col_number];
            col_number++;

            if( search_header && ( current_header == NULL || strcmp( to_lowercase( current_header ), to_lowercase( header_name ) ) != 0 ) ) {
                token = strtok( NULL, "," );
                continue;
            }

            if( strstr( to_lowercase( token ), to_lowercase( search_term ) ) != NULL ) {
                print_line = 1;
            }

            token = strtok( NULL, "," );
        }

        if( print_line == 1 || line_number == 0 ) {
            printf( "%s", line );
        }

        line_number++;
    }

    fclose( fp );

    return 0;
}
