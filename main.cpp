/*
 * RAY tracing EXAMPLE
 */

#include <cmath>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "canvas.h"
#include "mathHelper.h"
#include "object.h"
#include "world.h"
#include "camera.h"
#include "lightSource.h"
#include "illuminationModel.h"
#include "texture.h"


#include "kdtree.h"

// pixels
int imageHeight = 512;
int imageWidth = 512;

// keeping the aspect ratio of the window pixels
double viewPlaneHeigth = 0.5;
double viewPlaneWidth = 0.5;

int main ( void ) {
    // SFML canvas and window
    Canvas canvas( imageWidth, imageHeight );
    sf::RenderWindow window(sf::VideoMode(imageWidth, imageHeight), "Ray Tracer");

    // first create our objects
    Sphere frontSphere( Point(0.0,0.1,-1.8), 0.4, Color(1,1,1) );
    frontSphere.setUpPhong( Color(1,1,1), 0.075, 0.075, 0.2, 20.0 );
    frontSphere.setUpReflectionTransmission(0.01, 0.8, 0.95);

    Sphere backSphere( Point(-0.65,-0.2,-2.5), 0.3, Color(0.7,0.7,0.7) );
    backSphere.setUpPhong( Color(1,1,1), 0.15, 0.25, 1.0, 20.0 );
    backSphere.setUpReflectionTransmission(0.75, 0.0, 1.0);

    std::vector<Point> vertices;
    vertices.push_back( Point(-1.5,-0.6, 0.0) );
    vertices.push_back( Point(-1.5,-0.6,-6.0) );
    vertices.push_back( Point( 0.5,-0.6,-6.0) );
    vertices.push_back( Point( 0.5,-0.6, 0.0) );

    Polygon checkerFloor( vertices, Vector(0,1,0), planarCheckerTexture );
    checkerFloor.setUpPhong( Color(1,1,1), 0.3, 1.0, 0.0, 1.0 );

    // create a light source
    PointLight light( Point(0.0f, 5.0f, 3.0f), Color(1,1,1) );
    //PointLight light2( Point(-5.0f, 3.0f, 0.0f), Color(1,1,1) );
    //SpotLight light( Point(0.0, 2.0, -2.0), Color(1,1,1), Vector(0,-1,0), 20 );

    // create world, add objects in it
    World world;
    world.addObject(&frontSphere);
    world.addObject(&backSphere);
    world.addObject(&checkerFloor);

    // add light and set up phong
    world.addLight(&light);
    //world.addLight(&light2);
    //world.setUpPhongBlinnIllumination( Color(0.7,1,1) );
    world.setUpPhongIllumination( Color(0.25,0.61,1.00) );


    // TESTING KD TREES
    Voxel v(-2.0,5,-5,5,-5,5);
    std::vector<Object*> objectList;
    objectList.push_back(&frontSphere);
    objectList.push_back(&backSphere);

    Kdtree kd(objectList, v);

    //kd.walk(kd.getRoot());

    //Ray r(Point(0,0,10), Vector(0.1,-0.1,-1,true));
    //std::cout << std::boolalpha << v.intersect(r,0,10) << std::endl;




    // create camera
    Point pos(0,0,0);
    Vector lookAt(0,-0.5,0); // not being used yet
    Vector up(0,1,0); // not being used yet
    Camera cam(pos, lookAt, up, imageHeight, imageWidth, viewPlaneHeigth, viewPlaneWidth, 
        RAY_TRACER, 10, RAY_CENTER);

    // render our world, get the color map we will put on canvas
    std::vector<Color> colorMap = cam.render(world);

    // set pixel values on the canvas
    for(int i = 0; i < imageWidth; ++i) {
        for(int j = 0; j < imageHeight; ++j) {
            Color c = colorMap[i * imageWidth + j];
            canvas.setPixel( i, j, c.r, c.g, c.b );
        }
    }
    
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        canvas.draw(window);

        // end the current frame
        window.display();
    }

    return 0;
}