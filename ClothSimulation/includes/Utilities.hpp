/*
 * Some utility functions for the course TNM046
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2013-2014
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#pragma once
#include <glm/glm.hpp>
struct GLFWwindow;

namespace util {

/*
 * displayFPS() - Calculate, display and return frame rate statistics.
 * Called every frame, but statistics are updated only once per second.
 * The time per frame is a better measure of performance than the
 * number of frames per second, so both are displayed.
 *
 * NOTE: This function doesn't work properly if you call it for multiple
 * windows. Call it only for one window, and only once every frame.
 */
double displayFPS(GLFWwindow* window);



class KeyRotator {
public:
    KeyRotator(GLFWwindow* window);

    void poll();

    glm::vec3 move() const;

    double phi() const;
    double theta() const;

private:
    GLFWwindow* window_;

    double phi_;
    double theta_;
    double lastTime_;
};

class MouseRotator {
public:
    MouseRotator(GLFWwindow* window);

    void poll();

    double phi() const;
    double theta() const;
    glm::vec3 delta() const;
    
   
private:
    GLFWwindow* window_;

    double phi_;
    double theta_;

    double lastX_;
    double lastY_;
    
    bool leftPressed_;
    bool rightPressed_;
    double deltaX_;
    double deltaY_;
    double deltaZ_;
};



}  // namespace util
