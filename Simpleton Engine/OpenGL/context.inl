//
//  context.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <Simpleton/Platform/sdl error.hpp>

#include <utility>

GL::Context::Context(SDL_GLContext context)
  : context(context) {}

GL::Context::Context(Context &&other)
  : context(std::exchange(other.context, nullptr)) {}

GL::Context &GL::Context::operator=(Context &&other) {
  context = std::exchange(other.context, nullptr);
  return *this;
}

GL::Context::~Context() {
  SDL_GL_DeleteContext(context);
}

GL::Context GL::makeContext(SDL_Window *const window, const ContextParams &params) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, params.majorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, params.minorVersion);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, params.stencilBits);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, params.depthBits);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, params.colorBits);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  Context context(CHECK_SDL_NULL(SDL_GL_CreateContext(window)));
  CHECK_SDL_ERROR(SDL_GL_SetSwapInterval(params.vsync));
  
  glewExperimental = GL_TRUE;
  const GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    throw std::runtime_error(reinterpret_cast<const char *>(glewGetErrorString(glewError)));
  }
  
  CHECK_OPENGL_ERROR();
  
  return context;
}