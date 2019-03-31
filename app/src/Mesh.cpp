#include "Mesh.h"

#define AXIS_ALIGNS(axis)                                                      \
  (fabs(origin().axis - other.origin().axis) <                                 \
   scaling().axis + other.scaling().axis)

bool Mesh::checkAABB(const Mesh &other) {

  if (AXIS_ALIGNS(x)) {
    if (AXIS_ALIGNS(y)) {
      if (AXIS_ALIGNS(z)) {
        return true;
      }
    }
  }

  return false;
}
#undef AXIS_ALIGNS
