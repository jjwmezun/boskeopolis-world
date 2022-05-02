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
        std::string getHPString() const;
        void hurt( float damage );

    private:
        int gems;
        int shown_gems;
        bool treasures_collected[ MAX_TREASURES ];
        unsigned int pts_gfx;
        float health;
        float target_health;
        unsigned int hp_gfx;

        void updateGemsGraphics();
        void updateHPGraphics();
};

#endif // INVENTORY_H