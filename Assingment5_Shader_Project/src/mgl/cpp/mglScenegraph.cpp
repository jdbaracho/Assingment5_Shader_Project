////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// by João Baracho
//
////////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include "mglScenegraph.hpp"
#include "mglManager.hpp"

namespace mgl {

	////////////////////////////////////////////////////////////////////// IDrawable

	////////////////////////////////////////////////////////////////////////// Extra

	std::string vec3_to_string(glm::vec3 v) {
		return std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z);
	}

	glm::vec3 read_vec3(std::ifstream& file) {
		std::string line;
		glm::vec3 v(0.0f);

		std::getline(file, line);
		std::stringstream ss(line);
		ss >> v.x;
		ss >> v.y;
		ss >> v.z;
		return v;
	}

	std::string mat4_to_string(glm::mat4 m) {
		std::string s;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				s += std::to_string(m[i][j]);
				if (j < 3) s += " ";
			}
			if (i < 3) s += "\n";
		}
		return s;
	}

	glm::mat4 read_mat4(std::ifstream& file) {
		std::string line;
		glm::mat4 m(0.0f);

		for (int i = 0; i < 4; i++) {
			std::getline(file, line);
			std::stringstream ss(line);
			for (int j = 0; j < 4; j++) {
				ss >> m[i][j];
			}
		}
		return m;
	}

	///////////////////////////////////////////////////////////////////// Scenegraph

	Scenegraph::Scenegraph(std::string path) {
		this->path = "./assets/scenegraphs/" + path + ".txt";
	}

	Scenegraph::~Scenegraph() {}

	std::string Scenegraph::getPath() {
		return path;
	}

	void Scenegraph::createCamera(GLuint bindingpoint) {
		camera = new mgl::OrbitCamera(bindingpoint);
	}

	void Scenegraph::setCameraView(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
		viewMatrix[0] = eye;
		viewMatrix[1] = center;
		viewMatrix[2] = up;
		camera->setViewMatrix(eye, center, up);
	}

	void Scenegraph::setCameraPerspective(float fovy, float aspect, float near, float far) {
		projectionMatrix[0] = fovy;
		projectionMatrix[1] = aspect;
		projectionMatrix[2] = near;
		projectionMatrix[3] = far;
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

	void Scenegraph::save() {

		std::ofstream file(path);

		file << "Scenegraph" << std::endl;
		file << "eye:\n" << vec3_to_string(viewMatrix[0]) << std::endl;
		file << "center:\n" << vec3_to_string(viewMatrix[1]) << std::endl;
		file << "up:\n" << vec3_to_string(viewMatrix[2]) << std::endl;
		file << "fovy:\n" << projectionMatrix[0] << std::endl;
		file << "aspect:\n" << projectionMatrix[1] << std::endl;
		file << "near:\n" << projectionMatrix[2] << std::endl;
		file << "far:\n" << projectionMatrix[3] << std::endl;
		file << "light:\n" << vec3_to_string(light) << std::endl;

		for (auto& node : nodes) {
			node->save(file);
		}

		file.close();

		std::cout << "scenegraph saved on: " << path << std::endl;
	}

	void Scenegraph::load() {
		std::ifstream file;
		std::string line;

		file.open(path);
		if (file.fail()) {
			std::cout << "error: " << path << " does not exist" << std::endl;
			return;
		}

		// Scenegraph
		std::getline(file, line);

		// View Matrix
		for (auto& param : viewMatrix) {
			// name
			std::getline(file, line);
			param = read_vec3(file);
		}

		setCameraView(viewMatrix[0], viewMatrix[1], viewMatrix[2]);

		// Projection Matrix
		for (auto& param : projectionMatrix) {
			// name
			std::getline(file, line);
			// value
			std::getline(file, line);
			param = std::stof(line);
		}

		setCameraPerspective(projectionMatrix[0], projectionMatrix[1], projectionMatrix[2], projectionMatrix[3]);

		// Light Position
		std::getline(file, line);
		light = read_vec3(file);

		// Nodes
		while (std::getline(file, line)) {
			mgl::SceneNode* node = new mgl::SceneNode();
			node->load(file);
			addNode(node);
		}
		file.close();
		std::cout << "scenegraph loaded from: " << path << std::endl;
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

	void SceneNode::save(std::ofstream& file) {
		file << "Node" << std::endl;
		file << "modelMatrix:\n" << mat4_to_string(modelMatrix) << std::endl;
		file << "color:\n" << vec3_to_string(color) << std::endl;
		file << "meshID:\n" << meshID << std::endl;
		file << "shaderID:\n" << shaderID << std::endl;
	}

	void SceneNode::load(std::ifstream& file) {
		std::string line;

		// Model Matrix
		std::getline(file, line);
		modelMatrix = read_mat4(file);

		// Color
		std::getline(file, line);
		color = read_vec3(file);

		// MeshID
		std::getline(file, line);
		std::getline(file, line);
		meshID = line;

		// ShaderID
		std::getline(file, line);
		std::getline(file, line);
		shaderID = line;
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