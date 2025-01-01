#pragma once

#include <glad/glad.h>

#include <fmt/format.h>
#include <glfw/glfw3.h>
#include <stb_image.h>

unsigned int loadTexture(const char *filePath) {
  unsigned int texture; // NOLINT
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set the texture wrapping/filtering options (on the current bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load and generate the texture
  stbi_set_flip_vertically_on_load(1);
  int width, height, nrChannels; // NOLINT
  unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 4);

  if (data != nullptr) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    fmt::println("TEXTURE:: failed to load texture {}", filePath);
  }

  stbi_image_free(data);
  return texture;
}