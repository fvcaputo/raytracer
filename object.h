#ifndef _OBJECT_H
#define _OBJECT_H

#include <vector>
#include <cmath>
#include "mathHelper.h"

class Object {
protected:
    // material?
    Color col;
public:
    Object(Color col) : col(col) {}
    virtual Point intersect (Ray ray) = 0;// { return ray.getOrigin(); }
    virtual Color getColor () = 0;// { return Color(0,0,0); }
};

class Sphere : public Object {
    // Center and radius
    Point c;
    float r;

public:

    Sphere ( Point c, float r, Color col) : Object(col), c(c), r(r) {}

    Point intersect (Ray ray) {
        Point o = ray.getOrigin();
        Vector d = ray.getDirection();
        normalize(d);

        // a = 1 because direction of a ray is normalized
        //float A = 1;
        float B = 2.0f * (d.x * (o.x - c.x) + d.y * (o.y - c.y) + d.z * (o.z - c.z));
        float C = (o.x - c.x)*(o.x - c.x) + (o.y - c.y)*(o.y - c.y) + (o.z - c.z)*(o.z - c.z) - r*r;
        float w;

        if(B*B - 4.0f*C >= 0.0f) {
            float w1 = - (B + sqrt(B*B - 4.0f*C)) / 2;
            float w2 = - (B - sqrt(B*B - 4.0f*C)) / 2;

            if (w1 > 0 && w1 <= w2) // w1 is positive and smaller than or equal to w2
                w = w1;
            else if (w2 > 0) // w2 is positive and either smaller than w1 or w1 is negative
                w = w2;      

            return Point(o.x + d.x * w, o.y + d.y * w, o.z + d.z * w);       
        }

        return o; // will return the point origin if there is no intersection
    }

    Color getColor () {
        return col;
    }
};

class Polygon : public Object {
    // Polygon can have three or more vertices
    // here I assume they are definetly on the same plane
    std::vector<Point> vertices;

    // normal
    Vector n;

    // need shortest distance between origin and polygon for intersection
    float f;

public:

    // normal should be normalized before constructing the polygon
    Polygon ( std::vector<Point> vert , Vector n,  Color col) : Object(col), vertices(vert), n(n) {

        // for now I'm assuming all the vertices are on the same plane, and that place
        // has a normal (0,1,0), and all the points are on a plane parallel to the x and z plane
        // so f will always be y of any of the vertices (they are always the same)
        f = std::abs( vert[0].y );
    }



    Point intersect (Ray ray) {
        Point o = ray.getOrigin();
        Vector d = ray.getDirection();

        // ray-plane intersection
        float w = -(n.x*o.x + n.y*o.y + n.z*o.z + f) / (n.x*d.x + n.y*d.y + n.z*d.z);
        float wx, wy, wz;

        // there was a intersection
        if ( w >  0) {
            // actual intersection point
            wx = o.x + d.x * w;
            wy = o.y + d.y * w;
            wz = o.z + d.z * w;

            unsigned int i, j;
            bool result = false;
            for (i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
                if ( ((vertices[i].z > wz) != (vertices[j].z > wz)) && 
                    (wx < (vertices[j].x-vertices[i].x) * (wz-vertices[i].z) / (vertices[j].z-vertices[i].z) + vertices[i].x) )
                    result = !result;
            }

            if(result) 
                return Point(wx, wy, wz);
        }

        return o;
    }

    Color getColor () {
        return col;
    }
};


#endif