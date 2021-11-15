#ifndef INVENTORY_H
#define INVENTORY_H

enum class TreasureType
{
    GLOVES = 0
};

class Inventory
{
    public:
        static constexpr int MAX_TREASURES = 255;

        Inventory();
        void update();
        void addGems( int amount );
        int getShownGems() const;
        void getTreasure( int id );
        bool hasTreasure( TreasureType type ) const;

    private:
        int gems;
        int shown_gems;
        bool treasures_collected[ MAX_TREASURES ];
};

#endif // INVENTORY_H