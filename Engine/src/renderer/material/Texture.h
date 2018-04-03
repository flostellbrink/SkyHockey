#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>
#include <GL/glew.h>

class Texture {
 protected:
  Texture();
  void init();
 public:
  explicit Texture(GLuint handle);
  GLuint handle;
  int width, height;
  void bind();
};

#endif //TEXTURE_HPP