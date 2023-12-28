////////////////////////////////////////////////////////////////////////////////
//
// Orbit Camera Class
//
// by João Baracho
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <glm/gtx/quaternion.hpp>

#include "./mglOrbitCamera.hpp"

namespace mgl {



    ///////////////////////////////////////////////////////////////////////// ORBITCAMERA

    OrbitCamera::OrbitCamera(GLuint bindingpoint) : Camera(bindingpoint) {}

    void OrbitCamera::setViewMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
        d = glm::length(eye - center);
        T = glm::vec3(0, 0, -d);
        q = glm::toQuat(glm::lookAt(eye, center, up));
        Camera::setViewMatrix(glm::translate(T) * glm::toMat4(q));
    }

    void OrbitCamera::setPerspectiveMatrix(float _fovy, float _aspect, float _near, float _far) {
        fovy = _fovy;
        aspect = _aspect;
        near = _near;
        far = _far;
        setProjectionMatrix(glm::perspective(glm::radians(fovy), aspect, near, far));
    }

    void OrbitCamera::windowSize(float winx, float winy) {
        aspect = winx / winy;
        setProjectionMatrix(glm::perspective(glm::radians(fovy), aspect, near, far));
    }

    void OrbitCamera::update() {

        d += deltaScroll;
        d = d < minZoom ? minZoom : d > maxZoom ? maxZoom : d;
        T = glm::vec3(0, 0, -d);

        qX = glm::angleAxis(deltaX, XX);
        qY = glm::angleAxis(deltaY, YY);
        q = qX * qY * q;

        Camera::setViewMatrix(glm::translate(T) * glm::toMat4(q));
        setProjectionMatrix(getProjectionMatrix());

        deltaScroll = 0.0f;
        deltaX = 0.0f;
        deltaY = 0.0f;
    }

    void OrbitCamera::cursor(double xpos, double ypos) {

        if (leftClick) {
            deltaX += (xpos - prevXpos) * moveStep;
            deltaY += (ypos - prevYpos) * moveStep;
            prevXpos = xpos;
            prevYpos = ypos;
        }
    }

    void OrbitCamera::mouseButton(GLFWwindow* win, int button, int action) {

        leftClick = button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS;
        if (leftClick) {
            glfwGetCursorPos(win, &prevXpos, &prevYpos);
        }
    }

    void OrbitCamera::scroll(double xoffset, double yoffset) {

        deltaScroll -= yoffset * zoomStep;
    }

    ////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl
