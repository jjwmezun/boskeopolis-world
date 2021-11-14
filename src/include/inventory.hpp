#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory
{
    public:
        Inventory();
        void update();
        void addGems( int amount );
        int getShownGems() const;
        void getTreasure( int id );

    private:
        int gems;
        int shown_gems;
};

#endif // INVENTORY_H