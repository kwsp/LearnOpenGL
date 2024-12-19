#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <initializer_list>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

const char *const vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *const fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

unsigned int compileShader(int shaderType, const char *const shaderSource) {
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
      fmt::println("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog);
    } else {
      fmt::println("ERROR::SHADER::UNKNOWN::COMPILATION_FAILED\n{}", infoLog);
    }
  }
  return shader;
}

unsigned int createShaderProgram(std::initializer_list<unsigned int> shaders) {
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

int main(int argc, char *argv[]) {
  // glfw: init and config
  // ---------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw: window creation
  // ---------------------
  const int WIDTH = 800;
  const int HEIGHT = 600;

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    fmt::println("Failed to create GLFW window.");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fmt::println("Failed to initialize GLAD.");
    return -1;
  }

  // Build and compile our shader program
  // ------------------------------------
  auto vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  auto fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  auto shaderProgram = createShaderProgram({vertexShader, fragmentShader});
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Setup vertex data
  // -----------------
  std::vector<float> vertices = {
      // First triangle
      -0.5F, -0.5F, 0.0F, // left
      0.5F,  -0.5F, 0.0F, // right
      0.0F,  0.5F,  0.0F, // top
  };

  // Vertex array object (VAO)
  unsigned int VAO{};
  glGenVertexArrays(1, &VAO);

  // Vertex buffer object (VBO)
  unsigned int VBO{};
  glGenBuffers(1, &VBO);

  {
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy vertices array to a VBO
    // Array type of a vertex buffer object is GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);
    // 3. Then set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // Unbind the VAO
    glBindVertexArray(0);
  }

  // Render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // Input
    // -----
    processInput(window);

    // Render
    // ------
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glfw: swap buffers and poll IO events (keys, mouse)
    // ---------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Optional: terminate, clearing all allocated GLFW resources
  // ----------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // glfw: terminate
  // ---------------
  glfwTerminate();

  return 0;
}
