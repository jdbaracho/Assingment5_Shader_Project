////////////////////////////////////////////////////////////////////////////////
//
// Manager Class
//
// by João Baracho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_MANAGER_HPP
#define MGL_MANAGER_HPP

#include <string>
#include <map>

#include "./mglMesh.hpp"
#include "./mglShader.hpp"

namespace mgl {

    //////////////////////////////////////////////////////////////////////// MANAGER

    template<class E>
    class Manager {
    protected:
        Manager();
        std::map<std::string, E*> objects;

    public:
        static Manager<E>& getInstance();
        virtual ~Manager();

        void add(const std::string& key, E* object);
        E* get(const std::string& key);
        void display();
    };

    template<class E>
    Manager<E>& Manager<E>::getInstance() {
        static Manager<E> instance;
        return instance;
    }

    template<class E>
    Manager<E>::Manager() {}

    template<class E>
    Manager<E>::~Manager() {}

    template<class E>
    void Manager<E>::add(const std::string& key, E* object) {
        objects[key] = object;
    }

    template<class E>
    E* Manager<E>::get(const std::string& key) {
        return objects[key];
    }

    template<class E>
    void Manager<E>::display() {
        for (auto o : objects) {
            std::cout << "key: " << o.first << std::endl;
        }
    }

    ///////////////////////////////////////////////////////////////// SHADER MANAGER

    class ShaderManager : public Manager<ShaderProgram> {};

    /////////////////////////////////////////////////////////////////// MESH MANAGER

    class MeshManager : public Manager<Mesh> {};

    ////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MANAGER_HPP */