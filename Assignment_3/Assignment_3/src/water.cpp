#include "water.h"
#include "mygl/geometry.h"

Water waterCreate(const Vector4D& color)
{
    Water water;

    water.vertices.resize(grid::vertexPos.size());
    Vector4D colorAdjust(0.0, 0.0, 0.0, 0.0);
    for (unsigned i = 0; i < water.vertices.size(); i++) {
        water.vertices[i] = { grid::vertexPos[i], color + colorAdjust };
        colorAdjust += Vector4D(0.01, 0.01, 0.05, 0.0);
        if (i % 6 == 0) {
            colorAdjust = { 0.0, 0.0, 0.0, 0.0 };
        }
    }
    water.mesh = meshCreate(water.vertices, grid::indices, GL_DYNAMIC_DRAW, GL_STATIC_DRAW);
    return water;
}

void waterDelete(Water& water) { meshDelete(water.mesh); }
