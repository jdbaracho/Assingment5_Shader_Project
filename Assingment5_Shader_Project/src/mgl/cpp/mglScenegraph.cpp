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
#include "mglKeyBuffer.hpp"

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

	glm::vec3 Scenegraph::getS() {
		glm::mat4 view = camera->getViewMatrix();
		glm::vec3 s(view[0][0], view[1][0], view[2][0]);

		return glm::normalize(s);
	}

	glm::vec3 Scenegraph::getU() {
		glm::mat4 view = camera->getViewMatrix();
		glm::vec3 u(view[0][1], view[1][1], view[2][1]);

		return glm::normalize(u);
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
		node->setIndex(nodes.size());
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

	void Scenegraph::pick(GLFWwindow* win, int button, int action) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
			double xpos, ypos;
			int height;
			glfwGetCursorPos(win, &xpos, &ypos);
			glfwGetWindowSize(win, NULL, &height);
			glReadPixels(xpos, height - ypos, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &nodeID);
			std::cout << "picked object" << nodeID << std::endl;
		}
	}

	void Scenegraph::draw() {
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		camera->update();
		for (auto &node : nodes) {
			node->draw();
		}
		glDisable(GL_STENCIL_TEST);
	}

	void Scenegraph::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
		// change projection matrices to maintain aspect ratio
		camera->windowSize(winx, winy);
	}

	void Scenegraph::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
		KeyBuffer::getInstance().pressed[key] = action != GLFW_RELEASE;

		if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_KEY_ESCAPE:
				mode = Mode::NONE;
				std::cout << "mode deactivated" << std::endl;
				break;
			case GLFW_KEY_C:
				mode = Mode::CAMERA;
				std::cout << "camera mode activated" << std::endl;
				break;
			case GLFW_KEY_G:
				save();
				break;
			case GLFW_KEY_P:
				mode = Mode::PICK;
				std::cout << "pick mode activated" << std::endl;
				break;
			case GLFW_KEY_R:
				if (nodeID <= 0) {
					std::cout << "no item selected" << std::endl;
					break;
				}
				mode = Mode::ROTATE;
				std::cout << "rotate mode activated" << std::endl;
				break;
			case GLFW_KEY_S:
				if (nodeID <= 0) {
					std::cout << "no item selected" << std::endl;
					break;
				}
				mode = Mode::SCALE;
				std::cout << "scale mode activated" << std::endl;
				break;
			case GLFW_KEY_T:
				if (nodeID <= 0) {
					std::cout << "no item selected" << std::endl;
					break;
				}
				mode = Mode::TRANSLATE;
				std::cout << "translate mode activated" << std::endl;
				break;
			default:
				break;
			}
		}
	}

	void Scenegraph::cursorCallback(GLFWwindow* win, double xpos, double ypos) {

		switch (mode) {
		case Mode::CAMERA:
			camera->cursor(xpos, ypos);
			break;
		case Mode::ROTATE:
			if (!leftClick) break;
			nodes[nodeID - 1]->rotate(xpos - xprev, ypos - yprev);
				break;
		case Mode::TRANSLATE:
			if (!leftClick) break;
			nodes[nodeID - 1]->translate(xpos - xprev, ypos - yprev);
				break;
		default:
			break;
		}

		xprev = xpos;
		yprev = ypos;
	}

	void Scenegraph::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {

        leftClick = button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS;
		glfwGetCursorPos(win, &xprev, &yprev);

		switch (mode) {
		case Mode::CAMERA:
			camera->mouseButton(win, button, action);
			break;
		case Mode::PICK:
			pick(win, button, action);
			break;
		default:
			break;
		}
	}

	void Scenegraph::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
		switch (mode) {
		case Mode::CAMERA:
			camera->scroll(xoffset, yoffset);
			break;
		case Mode::SCALE:
			nodes[nodeID-1]->scale(yoffset);
		default:
			break;
		}
	}

	////////////////////////////////////////////////////////////////////// SceneNode

	SceneNode::SceneNode() {}

	SceneNode::~SceneNode() {}

	void SceneNode::setRoot(Scenegraph* root) {
		this->root = root;
	}

	void SceneNode::setIndex(int index) {
		this->index = index;
	}

	void SceneNode::setModelMatrix(glm::mat4 scale, glm::mat4 rotate, glm::mat4 translate) {
		modelMatrix[0] = scale;
		modelMatrix[1] = rotate;
		modelMatrix[2] = translate;
	}

	void SceneNode::updateModelMatrix(glm::mat4 scale, glm::mat4 rotate, glm::mat4 translate) {
		modelMatrix[0] = scale * modelMatrix[0];
		modelMatrix[1] = rotate * modelMatrix[1];
		modelMatrix[2] = translate * modelMatrix[2];
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
		file << "scale:\n" << mat4_to_string(modelMatrix[0]) << std::endl;
		file << "rotate:\n" << mat4_to_string(modelMatrix[1]) << std::endl;
		file << "translate:\n" << mat4_to_string(modelMatrix[2]) << std::endl;
		file << "color:\n" << vec3_to_string(color) << std::endl;
		file << "meshID:\n" << meshID << std::endl;
		file << "shaderID:\n" << shaderID << std::endl;
	}

	void SceneNode::load(std::ifstream& file) {
		std::string line;

		// Model Matrix
		for (auto& param : modelMatrix) {
			// name
			std::getline(file, line);
			param = read_mat4(file);
		}

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

	void SceneNode::scale(double amount) {
		KeyBuffer keys = KeyBuffer::getInstance();
		float sFactor = 1.0f;
		if (amount > 0) sFactor = scaleStep;
		if (amount < 0) sFactor = 1/scaleStep;

		glm::vec3 sVector(1.0f);
		if (keys.pressed[GLFW_KEY_X]) sVector.x = sFactor;
		else if (keys.pressed[GLFW_KEY_Y]) sVector.y = sFactor;
		else if (keys.pressed[GLFW_KEY_Z]) sVector.z = sFactor;
		else sVector = glm::vec3(sFactor);

		modelMatrix[0] = glm::scale(sVector) * modelMatrix[0];
	}

	void SceneNode::rotate(double xamount, double yamount) {
		glm::quat q = glm::toQuat(modelMatrix[1]);

		q = glm::angleAxis((float)(xamount * rotStep), root->getU()) * q;
		q = glm::angleAxis((float)(yamount * rotStep), root->getS()) * q;

		modelMatrix[1] = glm::toMat4(q);
	}

	void SceneNode::translate(double xamount, double yamount) {
		KeyBuffer keys = KeyBuffer::getInstance();
		glm::vec3 t(0.0f);
		glm::vec3 res(0.0f);

		t += root->getS() * (float)(xamount * transStep);
		t -= root->getU() * (float)(yamount * transStep);

		if (keys.pressed[GLFW_KEY_X]) res.x = t.x;
		else if (keys.pressed[GLFW_KEY_Y]) res.y = t.y;
		else if (keys.pressed[GLFW_KEY_Z]) res.z = t.z;
		else res = t;
		
		modelMatrix[2] = glm::translate(res) * modelMatrix[2];
	}

	void SceneNode::draw() {
		glStencilFunc(GL_ALWAYS, index + 1, 0xFF);

		ShaderProgram* shader = ShaderManager::getInstance().get(shaderID);

		shader->bind();

		GLint ModelMatrixId = shader->Uniforms[mgl::MODEL_MATRIX].index;
		glm::mat4 model = modelMatrix[2] * modelMatrix[1] * modelMatrix[0];
		glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(model));

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