const { SSL_OP_SSLEAY_080_CLIENT_DH_BUG } = require("constants");
const fs = require( `fs` );

const layerTypes = [
    `collision`,
    `bg1`,
    `bg2`,
    `bg3`,
    `fg1`,
    `fg2`,
    `fg3`,
    `bg1t`,
    `bg2t`,
    `bg3t`,
    `fg1t`,
    `fg2t`,
    `fg3t`
];

fs.readFile( `maps/blueberry-1.json`, ( err, data ) => {
    if ( err ) {
        throw err;
    }
    data = JSON.parse( data.toString() );
    const list = [];
    for ( const layer of data.layers )
    {
        let type = layerTypes.indexOf( layer.name );
        if ( type === -1 ) // Ignore invalid layers.
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