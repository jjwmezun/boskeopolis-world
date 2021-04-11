#include "graphic.hpp"

Graphic::~Graphic() {};

void Graphic::setId( int id )
{
    id_ = id;
};

int Graphic::getId() const
{
    return id_;
};