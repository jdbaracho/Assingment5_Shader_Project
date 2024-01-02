////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// by João Baracho
//
////////////////////////////////////////////////////////////////////////////////

#include "mglScenegraph.hpp"
#include "mglManager.hpp"

namespace mgl {

	////////////////////////////////////////////////////////////////////// IDrawable

	

	///////////////////////////////////////////////////////////////////// Scenegraph

	Scenegraph::Scenegraph() {}

	Scenegraph::~Scenegraph() {}

	void Scenegraph::setCamera(mgl::OrbitCamera* camera) {
		this->camera = camera;
	}

	void Scenegraph::addNode(SceneNode* node) {
		nodes.push_back(node);
	}

	void Scenegraph::draw() {
		for (auto &node : nodes) {
			node->draw();
		}
	}

	////////////////////////////////////////////////////////////////////// SceneNode

	SceneNode::SceneNode() {}

	SceneNode::~SceneNode() {}

	void SceneNode::setModelMatrix(glm::mat4 modelMatrix) {
		this->modelMatrix = modelMatrix;
	}

	void SceneNode::setMesh(std::string meshID) {
		this->meshID = meshID;
	}

	void SceneNode::setShader(std::string shaderID) {
		this->shaderID = shaderID;
	}

	void SceneNode::draw() {
		ShaderProgram* shader = ShaderManager::getInstance().get(shaderID);

		shader->bind();
		GLint ModelMatrixId = shader->Uniforms[mgl::MODEL_MATRIX].index;
		glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		MeshManager::getInstance().get(meshID)->draw();
		shader->unbind();
	}

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl