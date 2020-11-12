#ifndef BSWMATH_H
#define BSWMATH_H

#define XYTOI( w, x, y ) (( y ) * ( w ) + ( x ))
#define ITOX( w, i ) (( i ) % ( w ))
#define ITOY( w, i ) (( i ) / ( w ))

int min( int a, int b );

#endif