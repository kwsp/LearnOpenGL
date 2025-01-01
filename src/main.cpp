#include <glad/glad.h>

#include "Shader.hpp"
#include "Texture.hpp"
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
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
  Shader shader("shaders/shader.vert", "shaders/shader.frag");

  // Setup vertex data
  // -----------------
  std::vector<float> vertices = {
      // Position                     // Color                  // texture coord
      0.5F,  0.5F,  0.0F, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top left
      0.5F,  -0.5F, 0.0F, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5F, -0.5F, 0.0F, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5F, 0.5F,  0.0F, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top right
  };
  std::vector<unsigned int> indices = {
      0, 1, 3, // First triangle
      1, 2, 3, // Second triangle
  };

  std::vector<float> texCoords = {
      0.0f, 0.0f, // lower left corner
      1.0f, 0.0f, // lower right corner
      0.5f, 1.0f, // top center corner
  };

  auto texture1 = loadTexture("res/container.jpg");
  auto texture2 = loadTexture("res/awesomeface.png");

  // Vertex array object (VAO)
  unsigned int VAO{};
  glGenVertexArrays(1, &VAO);

  // Vertex buffer object (VBO)
  unsigned int VBO{};
  glGenBuffers(1, &VBO);

  // Element buffer object (EBO)
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  {
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy vertices array to a VBO
    // Array type of a vertex buffer object is GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // 3. Then set the vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);
  }

  // Render loop
  // -----------

  // Wireframe mode (for debug)
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  while (!glfwWindowShouldClose(window)) {
    // Input
    // -----
    processInput(window);

    // Render
    // ------
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate the shader
    shader.use();

    // Render triangle
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys, mouse)
    // ---------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Optional: terminate, clearing all allocated GLFW resources
  // ----------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  shader.deleteProgram();

  // glfw: terminate
  // ---------------
  glfwTerminate();

  return 0;
}
