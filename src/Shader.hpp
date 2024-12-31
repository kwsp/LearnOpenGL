#pragma once

#include <glad/glad.h>

#include <fmt/format.h>
#include <fstream>
#include <initializer_list>
#include <sstream>
#include <string>
#include <string_view>

class Shader {
public:
  unsigned int ID;

  // Construct the reader and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. Retrieve the vertex/fragment shader programs from filePath
    const std::string vertexCode = loadFilePath(vertexPath);
    const std::string fragmentCode = loadFilePath(fragmentPath);

    // 2. compile shaders
    const auto vertex = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    const auto fragment =
        compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    this->ID = createShaderProgram({vertex, fragment});

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  // Optional
  void deleteProgram() { glDeleteProgram(ID); }

  void use() { glUseProgram(ID); }

  void setBool(const std::string_view name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.data()), (int)value);
  }
  void setInt(const std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
  }
  void setFloat(const std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.data()), value);
  }

  static std::string loadFilePath(const char *filePath) {
    std::string sourceCode;
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      ifs.open(filePath);
      std::stringstream ss;
      ss << ifs.rdbuf();
      ifs.close();
      sourceCode = ss.str();
    } catch (std::ifstream::failure e) {
      fmt::println("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", filePath);
    }
    return sourceCode;
  }

  static unsigned int compileShader(int shaderType,
                                    const char *const shaderSource) {
    // Vertex shader
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success{};
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      if (shaderType == GL_VERTEX_SHADER) {
        fmt::println("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
      } else if (shaderType == GL_FRAGMENT_SHADER) {
        fmt::println("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}",
                     infoLog);
      } else {
        fmt::println("ERROR::SHADER::UNKNOWN::COMPILATION_FAILED\n{}", infoLog);
      }
    }
    return shader;
  }

  static unsigned int
  createShaderProgram(std::initializer_list<unsigned int> shaders) {
    // Link shader program
    unsigned int shaderProgram = glCreateProgram();
    for (auto shader : shaders) {
      glAttachShader(shaderProgram, shader);
    }

    glLinkProgram(shaderProgram);

    int success{};
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      fmt::println("ERROR::PROGRAM::SHADER link failed\n{}", infoLog);
    }

    return shaderProgram;
  }
};
