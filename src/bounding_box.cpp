#include <raylib.h>

#include "bounding_box.hpp"

BoundingBox::BoundingBox(Vector3 position, Vector3 size)
{
    this->position = position;
    this->size = size;
}

void BoundingBox::render() const
{
    DrawCubeWiresV(this->position, this->size, RED);
}
