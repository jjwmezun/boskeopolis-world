#ifndef FILENAME_H
#define FILENAME_H

void filename_init();
void filename_close();
char * filename_image( const char * local );
char * filename_map( const char * local );
char * filename_localization( const char * local );
char * filename_shader( const char * local );

#endif // FILENAME_H