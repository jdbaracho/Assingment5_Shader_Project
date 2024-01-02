////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>

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
		OrbitCamera* camera = nullptr;
		// light

		std::vector<SceneNode*> nodes;

	public:
		Scenegraph();
		~Scenegraph();
		void setCamera(OrbitCamera* camera);
		void addNode(SceneNode* node);

		void draw();
	};

	////////////////////////////////////////////////////////////////////// SceneNode

	class SceneNode : public IDrawable {
	private:
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		std::string meshID;
		std::string shaderID;
		// texture
		// callbacks

	public:
		SceneNode();
		~SceneNode();
		void setModelMatrix(glm::mat4 modelMatrix);
		void setMesh(std::string meshID);
		void setShader(std::string shaderID);

		void draw();
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */