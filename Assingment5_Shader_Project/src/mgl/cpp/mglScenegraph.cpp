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

	void Scenegraph::createCamera(GLuint bindingpoint) {
		camera = new mgl::OrbitCamera(bindingpoint);
	}

	void Scenegraph::setCameraView(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
		this->eye = eye;
		this->center = center;
		this->up = up;
		camera->setViewMatrix(eye, center, up);
	}

	void Scenegraph::setCameraPerspective(float fovy, float aspect, float near, float far) {
		this->fovy = fovy;
		this->aspect = aspect;
		this->near = near;
		this->far = far;
		camera->setPerspectiveMatrix(fovy, aspect, near, far);
	}

	void Scenegraph::setLight(glm::vec3 light) {
		this->light = light;
	}

	glm::vec3 Scenegraph::getLight() {
		return light;
	}

	void Scenegraph::addNode(SceneNode* node) {
		node->setRoot(this);
		nodes.push_back(node);
	}

	void Scenegraph::draw() {
		camera->update();
		for (auto &node : nodes) {
			node->draw();
		}
	}

	void Scenegraph::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
		// change projection matrices to maintain aspect ratio
		camera->windowSize(winx, winy);
	}

	void Scenegraph::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
		// pressedKeys[key] = action != GLFW_RELEASE;
	}

	void Scenegraph::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
		camera->cursor(xpos, ypos);
	}

	void Scenegraph::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
		camera->mouseButton(win, button, action);
	}

	void Scenegraph::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
		camera->scroll(xoffset, yoffset);
	}

	////////////////////////////////////////////////////////////////////// SceneNode

	SceneNode::SceneNode() {}

	SceneNode::~SceneNode() {}

	void SceneNode::setRoot(Scenegraph* root) {
		this->root = root;
	}

	void SceneNode::setModelMatrix(glm::mat4 modelMatrix) {
		this->modelMatrix = modelMatrix;
	}

	void SceneNode::updateModelMatrix(glm::mat4 modelMatrix) {
		this->modelMatrix = modelMatrix * this->modelMatrix;
	}

	void SceneNode::setColor(glm::vec3 color) {
		this->color = color;
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

		GLint ColorId = shader->Uniforms[mgl::COLOR_ATTRIBUTE].index;
		glUniform3f(ColorId, color.x, color.y, color.z);

		GLint LightPositionId = shader->Uniforms[mgl::LIGHT_POSITION].index;
		glm::vec3 light = root->getLight();
		glUniform3f(LightPositionId, light.x, light.y, light.z);

		MeshManager::getInstance().get(meshID)->draw();

		shader->unbind();
	}

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl