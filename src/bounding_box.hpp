#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include <raymath.h>

namespace engine
{
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
} // namespace engine

#endif // BOUNDING_BOX_H_