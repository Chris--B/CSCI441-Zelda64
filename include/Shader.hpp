#pragma once
#include "Utils.hpp"

class Shader {
public:
    GLint handle() const {
        assert(m_handle != -1);
        return m_handle;
    }

    void loadFromFile(const std::string &filename, GLenum kind);
    void loadFromString(const std::string &str, GLenum kind);

    // TODO: attributes and uniform setters and stuff.

private:
    GLint m_handle = -1;
};


struct ShaderProgram {
public:
    static void useFFS() { glUseProgram(0); }

    void create() {
        glChk();
        m_handle = glCreateProgram();
        glChk();
    }

    GLint handle() const {
        assert(m_handle != -1);
        return m_handle;
    }
    void link(const Shader &vert, const Shader &frag);

    void use() const { glUseProgram(handle()); }

private:
    GLint m_handle = -1;
};
