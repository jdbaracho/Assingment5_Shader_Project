////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-23 by Carlos Martinho
// 
// modified by João Baracho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <fstream>

#include "mglOrbitCamera.hpp"

namespace mgl {

	class IDrawable;
	class Scenegraph;
	class SceneNode;

	////////////////////////////////////////////////////////////////////// IDrawable

	class IDrawable {
	public:
		virtual void draw(void) = 0;
	};

	///////////////////////////////////////////////////////////////////// Scenegraph

	class Scenegraph : public IDrawable {
	private:
		std::string path;
		
		OrbitCamera* camera = nullptr;

		// View Matrix [eye, center, up]
		glm::vec3 viewMatrix[3];

		// Projection Matrix [fovy, aspect, near, far]
		float projectionMatrix[4];

		glm::vec3 light;

		std::vector<SceneNode*> nodes;

	public:
		Scenegraph(std::string path);
		~Scenegraph();
		std::string getPath();
		void createCamera(GLuint bindingpoint);
		void setCameraView(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
		void setCameraPerspective(float fovy, float aspect, float near, float far);
		void setLight(glm::vec3 light);
		glm::vec3 getLight();
		void addNode(SceneNode* node);

		void save();
		void load();

		void draw();

		void windowSizeCallback(GLFWwindow* win, int width, int height);
		void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
		void cursorCallback(GLFWwindow* win, double xpos, double ypos);
		void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods);
		void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);
	};

	////////////////////////////////////////////////////////////////////// SceneNode

	class SceneNode : public IDrawable {
	private:
		Scenegraph* root = nullptr;
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		std::string meshID;
		std::string shaderID;
		// texture
		// callbacks

	public:
		SceneNode();
		~SceneNode();
		void setRoot(Scenegraph* root);
		void setModelMatrix(glm::mat4 modelMatrix);
		void updateModelMatrix(glm::mat4 modelMatrix);
		void setColor(glm::vec3 color);
		void setMesh(std::string meshID);
		void setShader(std::string shaderID);

		void save(std::ofstream& file);
		void load(std::ifstream& file);

		void draw();
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */