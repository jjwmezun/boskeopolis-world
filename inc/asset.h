#ifndef ASSET_H
#define ASSET_H

void asset_init();
void asset_close();
char * asset_map( const char * local );
char * asset_image( const char * local );
char * asset_read_file_text( const char * filename );

#endif