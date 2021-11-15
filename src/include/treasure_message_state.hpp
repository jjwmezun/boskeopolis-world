#ifndef TREASURE_MESSAGE_STATE_H
#define TREASURE_MESSAGE_STATE_H

class TreasureMessageState
{
    public:
        TreasureMessageState( int id );
        void update();
        void init( unsigned int state );

    private:
        int id;
};

#endif // TREASURE_MESSAGE_STATE_H