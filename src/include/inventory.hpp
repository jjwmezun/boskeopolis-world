#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

enum class TreasureType
{
    GLOVES = 0
};

class Inventory
{
    public:
        static constexpr int MAX_TREASURES = 255;

        Inventory();
        void init( unsigned int state );
        void update();
        void addGems( int amount );
        int getShownGems() const;
        void getTreasure( int id );
        bool hasTreasure( TreasureType type ) const;
        std::string getShownGemsString() const;

    private:
        int gems;
        int shown_gems;
        bool treasures_collected[ MAX_TREASURES ];
        unsigned int pts_gfx;

        void updateGemsGraphics();
};

#endif // INVENTORY_H