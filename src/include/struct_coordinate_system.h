#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

/**
 * Struct representing a point in a 3D Cartesian coordinate system.
 * 
 * @x: X coordinate
 * @y: Y coordinate
 * @z: Z coordinate
 */
typedef struct {
    float x;
    float y;
    float z;
} cartesian_point;

/**
 * Struct representing a point in a cylindrical coordinate system.
 * 
 * @radius: Radius from the origin
 * @angle: Angle in degrees from the positive X-axis in the XY plane
 * @height: Height from the base
 */
typedef struct {
    float radius;
    float angle;
    float height;
} cylindrical_point;


#endif // COORDINATE_SYSTEM_H