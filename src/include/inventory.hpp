#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory
{
    public:
        Inventory();
        void update();
        void addGems( int amount );
        int getShownGems() const;

    private:
        int gems;
        int shown_gems;
};

#endif // INVENTORY_H