const { SSL_OP_SSLEAY_080_CLIENT_DH_BUG } = require("constants");
const fs = require( `fs` );

fs.readFile( `maps/blueberry-1.json`, ( err, data ) => {
    if ( err ) {
        throw err;
    }
    data = JSON.parse( data.toString() );
    const list = [];
    for ( const layer of data.layers )
    {
        let type;
        if ( layer.name === `collision` )
        {
            type = 0;
        }
        else if ( layer.name === `bg` )
        {
            type = 1;
        }
        else
        {
            continue;
        }
        const width = layer.width;
        const height = layer.height;
        list.push( type );
        list.push( width );
        list.push( height );
        for ( const tile of layer.data )
        {
            list.push( tile );
        }
    }
    const buffer = new Int16Array( list.length );
    for ( let i = 0; i < list.length; ++i ) {
        buffer[ i ] = list[ i ];
    }
    fs.writeFile( `../bin/assets/maps/blueberry-1.bswm`, buffer, ( err ) => {
        if ( err ) {
            throw err;
        }
    } );
});