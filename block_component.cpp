#include "block_component.hpp"
#include "map.hpp"

namespace BSW
{
    BlockComponent::BlockComponent( Type type, const ArgList & args )
        : type_ ( type )
    {
        switch ( type_ )
        {
            case ( Type::MONEY ):
            {
                float amount = static_cast<float> ( GetArg( "amount", args, 100 ) );
                data_.money.amount = amount;
            }
            break;
        }
    };

    void BlockComponent::interact( Sprite & sprite, Level & level, Game & game, Map & map, Block & block ) const
    {
        switch ( type_ )
        {
            case ( Type::MONEY ):
            {
                //level.inventory().addFunds( data_.money.amount );
            }
            break;
            case ( Type::REMOVE ):
            {
                //map.scheduleBlockRemoval();
            }
            break;
        }
    };
}