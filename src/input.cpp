#include "input.hpp"

namespace Input
{
    std::unordered_map<Key, std::vector<int>> keys;
    std::unordered_map<int, std::vector<Key>> raw_keys;
    //std::unordered_map<int, std::vector<std::function<void ()>>> actions;
    bool held[ NUMBER_OF_KEYS ];
    bool pressed[ NUMBER_OF_KEYS ];
    bool press_locks[ NUMBER_OF_KEYS ];

    void init()
    {
        for ( int i = 0; i < NUMBER_OF_KEYS; ++i )
        {
            held[ i ] = false;
            pressed[ i ] = false;
            press_locks[ i ] = false;
        }
    };

    void initKeys( std::unordered_map<Key, std::vector<int>> new_keys )
    {
        keys = new_keys;
        for ( auto key : keys )
        {
            auto & raw_key_list = key.second;
            for ( int raw_key : raw_key_list )
            {
                auto iterator = raw_keys.find( raw_key );
                if ( iterator == raw_keys.end() )
                {
                    raw_keys.insert( std::pair<int, std::vector<Key>>{ raw_key, { key.first } } );
                }
                else
                {
                    iterator->second.emplace_back( key.first );
                }
            }
        }
    };

    void press( int raw_key )
    {
        auto iterator = raw_keys.find( raw_key );
        if ( iterator != raw_keys.end() )
        {
            for ( Key key : iterator->second )
            {
                int key_num = ( int )( key );
                held[ key_num ] = true;
                if ( !press_locks[ key_num ] )
                {
                    pressed[ key_num ] = press_locks[ key_num ] = true;
                }
            }
        }
    };

    void release( int raw_key )
    {
        auto iterator = raw_keys.find( raw_key );
        if ( iterator != raw_keys.end() )
        {
            for ( Key key : iterator->second )
            {
                int key_num = ( int )( key );
                held[ key_num ] = press_locks[ key_num ] = false;
            }
        }
    };

    void update()
    {
        for ( int i = 0; i < NUMBER_OF_KEYS; ++i )
        {
            pressed[ i ] = false;
        }
    };

    /*
    void registerPressAction( Key key, std::function<void ()> action )
    {
        auto iterator = keys.find( key );
        if ( iterator != keys.end() )
        {
            for ( int raw_key : iterator->second )
            {
                auto action_list_iterator = actions.find( raw_key );
                if ( action_list_iterator == actions.end() )
                {
                    actions.insert( std::pair<int, std::vector<std::function<void ()>>> { raw_key, { action } } );
                }
                else
                {
                    action_list_iterator->second.emplace_back( action );
                }
            }
        }
    };*/

    bool heldRight()
    {
        return held[ ( int )( Key::RIGHT ) ];
    };

    bool heldLeft()
    {
        return held[ ( int )( Key::LEFT ) ];
    };

    bool pressedRight()
    {
        return pressed[ ( int )( Key::RIGHT ) ];
    };

    bool pressedLeft()
    {
        return pressed[ ( int )( Key::RIGHT ) ];
    };
};