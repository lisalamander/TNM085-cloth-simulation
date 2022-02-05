/*
 * Some utility functions for the course TNM046
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2014
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#include "../includes/Utilities.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <corecrt_math_defines.h>


namespace util {

double displayFPS(GLFWwindow* window) {
    static int frames = 0;
    static double fps = 0.0;

    static double t0 = glfwGetTime();  // Gets number of seconds since glfwInit()

    double t = glfwGetTime();  // Get current time

    // update fps only once every second
    if (t - t0 >= 1.0) {
        fps = static_cast<double>(frames) / (t - t0);
        t0 = t;
        frames = 0;
    }

    // update the window title
    if (frames == 0) {
        char title[201];
        // convert fps to milliseconds
        double frametime = (fps > 0.0) ? 1000.0 / fps : 0.0;
        snprintf(title, 200, "TNM046: %.2f ms/frame (%.1f FPS)", frametime, fps);
        glfwSetWindowTitle(window, title);
    }

    ++frames;
    return fps;
}


KeyRotator::KeyRotator(GLFWwindow* window)
    : window_(window), phi_(0.0), theta_(0.0), lastTime_(glfwGetTime()) {}

void KeyRotator::poll() {
    const double currentTime = glfwGetTime();
    const double elapsedTime = currentTime - lastTime_;
    lastTime_ = currentTime;

    if (glfwGetKey(window_, GLFW_KEY_RIGHT)) {
        phi_ += elapsedTime * M_PI / 2.0;  // Rotate 90 degrees per second (pi/2)
        phi_ = fmod(phi_, M_PI * 2.0);     // Wrap around at 360 degrees (2*pi)
    }

    if (glfwGetKey(window_, GLFW_KEY_LEFT)) {
        phi_ -= elapsedTime * M_PI / 2.0;  // Rotate 90 degrees per second (pi/2)
        phi_ = fmod(phi_, M_PI * 2.0);
        if (phi_ < 0.0) {
            phi_ += M_PI * 2.0;  // If phi_<0, then fmod(phi_,2*pi)<0
        }
    }

    if (glfwGetKey(window_, GLFW_KEY_UP)) {
        theta_ += elapsedTime * M_PI / 2.0;  // Rotate 90 degrees per second
        if (theta_ >= M_PI / 2.0) {
            theta_ = M_PI / 2.0;  // Clamp at 90
        }
    }

    if (glfwGetKey(window_, GLFW_KEY_DOWN)) {
        theta_ -= elapsedTime * M_PI / 2.0;  // Rotate 90 degrees per second
        if (theta_ < -M_PI / 2.0) {
            theta_ = -M_PI / 2.0;  // Clamp at -90
        }
    }
}

double KeyRotator::phi() const { return phi_; }

double KeyRotator::theta() const { return theta_; }

MouseRotator::MouseRotator(GLFWwindow* window)
    : window_(window), phi_(0.0), theta_(0.0), leftPressed_(false), rightPressed_(false) {
    glfwGetCursorPos(window, &lastX_, &lastY_);
}

void MouseRotator::poll() {
    // Find out where the mouse pointer is, and which buttons are pressed
    double currentX;
    double currentY;
    glfwGetCursorPos(window_, &currentX, &currentY);

    bool currentLeft = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool currentRight = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (currentLeft && leftPressed_) {  // If a left button drag is in progress
        int windowWidth;
        int windowHeight;
        glfwGetWindowSize(window_, &windowWidth, &windowHeight);

        const double moveX = currentX - lastX_;
        const double moveY = currentY - lastY_;
        deltaX_ = moveX;
        deltaY_ = -moveY;

        phi_ += M_PI * moveX / windowWidth;  // Longest drag rotates 180 degrees
        if (phi_ >= M_PI * 2.0) {
            phi_ = fmod(phi_, M_PI * 2.0);
        }
        if (phi_ < 0.0) {
            phi_ += M_PI * 2.0;
        }                                       // If phi<0, then fmod(phi,2*pi)<0
        theta_ += M_PI * moveY / windowHeight;  // Longest drag rotates 180 deg
        if (theta_ >= M_PI / 2.0) {
            theta_ = M_PI / 2.0;  // Clamp at 90
        }
        if (theta_ < -M_PI / 2.0) {
            theta_ = -M_PI / 2.0;  // Clamp at 90
        }
    }
    if (currentRight && rightPressed_) {
        deltaZ_ = currentX - lastX_;
    }
    leftPressed_ = currentLeft;
    rightPressed_ = currentRight;
    lastX_ = currentX;
    lastY_ = currentY;
}

double MouseRotator::phi() const { return phi_; }

double MouseRotator::theta() const { return theta_; }

glm::vec3 MouseRotator::delta() const { return glm::vec3(deltaX_, deltaY_, deltaZ_); }

}  // namespace util


