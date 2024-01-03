////////////////////////////////////////////////////////////////////////////////
//
// Shader Project
//
// Copyright (c) 2023 by Carlos Martinho
//
// modified by João Baracho
//
////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "../mgl/mgl.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;
    void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) override;
    void cursorCallback(GLFWwindow* win, double xpos, double ypos) override;
    void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override;
    void scrollCallback(GLFWwindow* win, double xoffset, double yoffset) override;

private:
    const GLuint UBO_BP = 0;
    mgl::OrbitCamera* Camera = nullptr;
    mgl::Scenegraph* scenegraph = nullptr;

    void newMesh(std::string meshID, std::string meshFile);
    void createMeshes();
    void newShader(std::string meshID, std::string shaderID, std::string vShader, std::string fShader);
    void createShaderPrograms();
    void createCamera();
    void createScenegraph();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::newMesh(std::string meshID, std::string meshFile) {
    std::string path = "./assets/models/" + meshFile;

    mgl::Mesh* mesh = new mgl::Mesh();
    mesh->joinIdenticalVertices();
    mesh->create(path);

    mgl::MeshManager::getInstance().add(meshID, mesh);
}

void MyApp::createMeshes() {
    newMesh("cube", "cube-vtn-2.obj");
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::newShader(std::string meshID, std::string shaderID, std::string vShader, std::string fShader) {
    mgl::Mesh* mesh = mgl::MeshManager::getInstance().get(meshID);

    mgl::ShaderProgram* shader = new mgl::ShaderProgram();
    shader->addShader(GL_VERTEX_SHADER, "./src/shaders/" + vShader);
    shader->addShader(GL_FRAGMENT_SHADER, "./src/shaders/" + fShader);

    shader->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    if (mesh->hasNormals()) {
        shader->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
    }
    if (mesh->hasTexcoords()) {
        shader->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
    }
    if (mesh->hasTangentsAndBitangents()) {
        shader->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
    }

    shader->addUniform(mgl::MODEL_MATRIX);
    shader->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
    shader->create();

    mgl::ShaderManager::getInstance().add(shaderID, shader);
}

void MyApp::createShaderPrograms() {
    newShader("cube", "base", "cube-vs.glsl", "cube-fs.glsl");
}

///////////////////////////////////////////////////////////////////////// CAMERA

void MyApp::createCamera() {
    Camera = new mgl::OrbitCamera(UBO_BP);
    // Eye(5,5,5) Center(0,0,0) Up(0,1,0)
    Camera->setViewMatrix(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(100)
    Camera->setPerspectiveMatrix(30.0f, 640.0f / 480.0f, 1.0f, 100.0f);
}

///////////////////////////////////////////////////////////////////// SCENEGRAPH

void MyApp::createScenegraph() {
    scenegraph = new mgl::Scenegraph();

    mgl::SceneNode* node = new mgl::SceneNode();
    // scale(0.5)
    node->updateModelMatrix(glm::scale(glm::vec3(0.5f)));
    node->setMesh("cube");
    node->setShader("base");
    scenegraph->addNode(node);

    node = new mgl::SceneNode();
    // scale(0.5)
    node->updateModelMatrix(glm::scale(glm::vec3(0.2f)));
    // rotate(45º, XX)
    node->updateModelMatrix(glm::rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    // rotate(45º, ZZ)
    node->updateModelMatrix(glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    // translate(0, 1, 0)
    node->updateModelMatrix(glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    node->setMesh("cube");
    node->setShader("base");
    scenegraph->addNode(node);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    createMeshes();
    createShaderPrograms();  // after mesh;
    createCamera();
    createScenegraph();
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
    // change projection matrices to maintain aspect ratio
    Camera->windowSize(winx, winy);
}

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    // pressedKeys[key] = action != GLFW_RELEASE;
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) {
    Camera->update();
    scenegraph->draw();
}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
    Camera->cursor(xpos, ypos);
}

void MyApp::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
    Camera->mouseButton(win, button, action);
}

void MyApp::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
    Camera->scroll(xoffset, yoffset);
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
    mgl::Engine& engine = mgl::Engine::getInstance();
    engine.setApp(new MyApp());
    engine.setOpenGL(4, 6);
    engine.setWindow(800, 600, "Shader Project", 0, 1);
    engine.init();
    engine.run();
    exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////