//
//  Arcball.h
//
//  Created by Saburo Okita on 12/03/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Arcball__Arcball__
#define __Arcball__Arcball__ 

#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Arcball {
private:
    int windowWidth;
    int windowHeight;
    int mouseEvent;
    GLfloat rollSpeed;
    GLfloat angle ;
    glm::vec3 prevPos;
    glm::vec3 currPos;
    glm::vec3 camAxis;
    
    bool xAxis;
    bool yAxis;
    
public:
    
    /**
     * Constructor.
     * @param roll_speed: the speed of rotation
     */
    Arcball(int window_width, int window_height, GLfloat roll_speed, bool x_axis, bool y_axis)
    {
        init(window_width, window_height, roll_speed, x_axis, y_axis);
    }
    
    /**
     * Init: all initialization
     */
    void init(int window_width, int window_height, GLfloat roll_speed, bool x_axis, bool y_axis)
    {
        this->windowWidth  = window_width;
        this->windowHeight = window_height;
        
        this->mouseEvent = 0;
        this->rollSpeed  = roll_speed;
        this->angle      = 0.0f;
        this->camAxis    = glm::vec3(0.0f, 1.0f, 0.0f);
        
        this->xAxis = x_axis;
        this->yAxis = y_axis;
    }
    
    /**
     * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
     * into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
     */
    glm::vec3 toScreenCoord( double x, double y ) {
        glm::vec3 coord(0.0f);
        
        if( xAxis )
            coord.x =  (2 * (float)x - windowWidth ) / windowWidth;
        
        if( yAxis )
            coord.y = -(2 * (float)y - windowHeight) / windowHeight;
        
        /* Clamp it to border of the windows, comment these codes to allow rotation when cursor is not over window */
        coord.x = glm::clamp( coord.x, -1.0f, 1.0f );
        coord.y = glm::clamp( coord.y, -1.0f, 1.0f );
        
        float length_squared = coord.x * coord.x + coord.y * coord.y;
        if( length_squared <= 1.0 )
            coord.z = sqrt( 1.0 - length_squared );
        else
            coord = glm::normalize( coord );
        
        return coord;
    }
    
    /**
     * Check whether we should start the mouse event
     * Event 0: when no tracking occured
     * Event 1: at the start of tracking, recording the first cursor pos
     * Event 2: tracking of subsequent cursor movement
     */
    void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
        mouseEvent = ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT );
    }
    
    void cursorCallback( GLFWwindow *window, double x, double y ){
        if( mouseEvent == 0 )
            return;
        else if( mouseEvent == 1 ) {
            /* Start of trackball, remember the first position */
            prevPos     = toScreenCoord( x, y );
            mouseEvent  = 2;
            return;
        }
        
        /* Tracking the subsequent */
        currPos  = toScreenCoord( x, y );
        
        /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
        //angle    = acos( std::min(1.0f, glm::dot(prevPos, currPos) ));
        float dot = (float)glm::dot(prevPos, currPos);
        angle = acos(((1.0f < dot) ? 1.0f : dot));
        
        /* Cross product to get the rotation axis */
        camAxis  = glm::cross( prevPos, currPos );
    }
    
    /**
     * Create rotation matrix
     * multiply this matrix with view (or model) matrix to rotate the camera (or model)
     */
    glm::mat4 createRotationMatrix() {
        return glm::rotate( glm::degrees(angle) * rollSpeed, camAxis );
    }
    
};

#endif /* defined(__Arcball__Arcball__) */
