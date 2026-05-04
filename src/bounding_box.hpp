#ifndef BOUDING_BOX_H_
#define BOUDING_BOX_H_

#include <raymath.h>

class BoundingBox
{
  public:
    BoundingBox() = default;

    BoundingBox(Vector3 position, Vector3 size);

    ~BoundingBox() = default;

    Vector3 position;
    Vector3 size;

    void render() const;

  private:
};

#endif // BOUDING_BOX_H_