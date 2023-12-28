////////////////////////////////////////////////////////////////////////////////
//
// Orbit Camera Class
//
// by João Baracho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_ORBIT_CAMERA_HPP
#define MGL_ORBIT_CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "./mglApp.hpp"
#include "./mglCamera.hpp"
#include "./mglConventions.hpp"

namespace mgl {

	class OrbitCamera;

	///////////////////////////////////////////////////////////////////////// Camera

	class OrbitCamera : public mgl::Camera {
	private:
		// ORBIT
		bool leftClick = false;
		double prevXpos, prevYpos;
		float deltaX = 0.0f, deltaY = 0.0f;
		const float moveStep = 0.01;

		// ZOOM
		float deltaScroll = 0.0f;
		const float zoomStep = 0.1;
		const float minZoom = 1.0f;
		const float maxZoom = 10.0f;

		// AXIS
		const glm::vec3 XX = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3 YY = glm::vec3(1.0f, 0.0f, 0.0f);

		// VIEW MATRIX
		float d;
		glm::vec3 T;
		glm::quat q;
		glm::quat qX;
		glm::quat qY;

		// PROJECTION MATRIX
		float fovy;
		float aspect;
		float near;
		float far;

	public:
		explicit OrbitCamera(GLuint bindingpoint);
		void setViewMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
		void setPerspectiveMatrix(float fovy, float aspect, float near, float far);

		void update();
		void windowSize(float winx, float winy);
		void cursor(double xpos, double ypos);
		void mouseButton(GLFWwindow* win, int button, int action);
		void scroll(double xoffset, double yoffset);
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_ORBIT_CAMERA_HPP */
