#include "Framebuffer.h"
#include <utility>
#include <src/support/Formatter.h>
Framebuffer::Framebuffer(GLuint handle, std::shared_ptr<Texture> texture)
    : handle_(handle) {
  textures.push_back(std::move(texture));
}

Framebuffer::Framebuffer(int width, int height, int texture_count, bool depth, bool mipmap, std::string fb_name) :
    width(width),
    height(height),
    texture_count(texture_count),
    draw_buffers(texture_count, 0){
  glGenFramebuffers(1, &handle_);
  glBindFramebuffer(GL_FRAMEBUFFER, handle_);

  if (depth) {
    GLuint depth_buffer;
    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
  }

  for (int texture_id = 0; texture_id < texture_count; texture_id++) {
    auto attachment = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + texture_id);
    draw_buffers[texture_id] = attachment;

    GLuint texture_handle;
    glGenTextures(1, &texture_handle);
    glBindTexture(GL_TEXTURE_2D, texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

    if (mipmap) {
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture_handle, 0);

    glObjectLabel(GL_TEXTURE, texture_handle, -1, (fb_name + std::to_string(texture_id)).c_str());

    textures.push_back(std::make_shared<Texture>(texture_handle));
  }
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, handle_);
  glDrawBuffers(texture_count, &draw_buffers[0]);
  glViewport(0, 0, width, height);
}
