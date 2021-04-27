#ifndef GRAPHIC_H
#define GRAPHIC_H

class Graphic
{
    public:
        virtual ~Graphic();
        virtual void render() const = 0;
        void setId( int id );
        int getId() const;

    private:
        int id_;
};

#endif // GRAPHIC_H