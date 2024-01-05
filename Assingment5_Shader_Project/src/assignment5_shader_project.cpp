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
    mgl::Scenegraph* scenegraph = nullptr;

    void cubeMesh();
    void createMeshes();
    void phongShader();
    void createShaderPrograms();
    void createScenegraph(bool reset);
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::cubeMesh() {
    std::string path = "./assets/models/cube-vtn.obj";

    mgl::Mesh* mesh = new mgl::Mesh();
    mesh->joinIdenticalVertices();
    mesh->create(path);

    mgl::MeshManager::getInstance().add("cube", mesh);
}

void MyApp::createMeshes() {
    cubeMesh();
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::phongShader() {

    mgl::ShaderProgram* shader = new mgl::ShaderProgram();
    shader->addShader(GL_VERTEX_SHADER, "./src/shaders/phong-vs.glsl");
    shader->addShader(GL_FRAGMENT_SHADER, "./src/shaders/phong-fs.glsl");

    shader->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    shader->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);

    shader->addUniform(mgl::MODEL_MATRIX);
    shader->addUniform(mgl::COLOR);
    shader->addUniform(mgl::LIGHT_POSITION);
    shader->addUniform(mgl::EYE_POSITION);
    shader->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
    shader->create();

    mgl::ShaderManager::getInstance().add("phong", shader);
}

void MyApp::createShaderPrograms() {
    phongShader();
}

///////////////////////////////////////////////////////////////////// SCENEGRAPH

glm::mat4 I(1.0f);
glm::mat4 S(1.0f);
glm::mat4 R(1.0f);
glm::mat4 T(1.0f);

void MyApp::createScenegraph(bool reset) {
    scenegraph = new mgl::Scenegraph("scenepraph1");
    scenegraph->createCamera(UBO_BP);

    if (!reset && scenegraph->load()) {
        return;
    }

    // Eye(5,5,5) Center(0,0,0) Up(0,1,0)
    scenegraph->setCameraView(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(100)
    scenegraph->setCameraPerspective(30.0f, 640.0f / 480.0f, 1.0f, 100.0f);

    // LIGHT
    // position(6, 5, 10)
    scenegraph->setLight(glm::vec3(6.0f, 5.0f, 10.0f));

    // NODES
    mgl::SceneNode* node = new mgl::SceneNode();
    // scale(0.5)
    S = glm::scale(glm::vec3(0.5f));
    node->setModelMatrix(S, I, I);

    node->setMesh("cube");
    node->setShader("phong");
    scenegraph->addNode(node);

    node = new mgl::SceneNode();
    // scale(0.5)
    S = glm::scale(glm::vec3(0.2f));
    // rotate(45º, (1, 1, 1))
    R = glm::rotate(glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // translate(0, 1, 0)
    T = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
    node->setModelMatrix(S, R, T);

    // color(red)
    node->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    node->setMesh("cube");
    node->setShader("phong");
    scenegraph->addNode(node);

    std::cout << "scenegraph created" << std::endl;
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    createMeshes();
    createShaderPrograms();  // after mesh;
    createScenegraph(false);
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
    // change projection matrices to maintain aspect ratio
    scenegraph->windowSizeCallback(win, winx, winy);
}

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    scenegraph->keyCallback(win, key, scancode, action, mods);

    if (action == GLFW_RELEASE) {
        switch (key) {
        case GLFW_KEY_H:
            std::cout << "reset scenegraph" << std::endl;
            createScenegraph(true);
            break;
        default:
            break;
        }
    }
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) {
    scenegraph->draw();
}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
    scenegraph->cursorCallback(win, xpos, ypos);
}

void MyApp::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
    scenegraph->mouseButtonCallback(win, button, action, mods);
}

void MyApp::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
    scenegraph->scrollCallback(win, xoffset, yoffset);
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