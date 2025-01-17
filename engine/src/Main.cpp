#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

#define INCLUDE_SDL
#include <glm/gtc/matrix_transform.hpp>

#include "InputManager.h"
#include "SDL_include.h"
// #include "Texture.h"
#include "BufferSet.hpp"
#include "Circle.hpp"
#include "Model.hpp"
#include "Queengine.h"
#include "Rectangle.hpp"
#include "Shader.h" // #include "TextureLoader.h"
#include "Triangle.hpp"
#include "vboindexer.hpp"

#include "../include/log.h"

using namespace std;

int main(int argc, char **argv) {
  Queengine *engine = Queengine::GetInstance();

  // std::vector<glm::vec3> vertices;
  // std::vector<glm::vec2> tex_coords;
  // std::vector<glm::vec3> normal;
  // std::vector<unsigned int> indices;
  // std::vector<float> light = {1, 0, 0};

  // bool fileOpened = load3DOBJ("engine/assets/obj/suzanne.obj", vertices,
  // tex_coords, normal);

  Material *material = (Material *)malloc(sizeof(Material));
  bool mtl = loadMtl(material, "engine/assets/obj/suzanne.mtl");
  // for(int i = 0; i < vertices.size();i++) {
  //  indices.push_back(i);
  //}

  // std::vector<glm::vec3> vertices = primitiva.get_coordinates();
  // std::vector<unsigned int> indices = primitiva.get_indices();
  // std::vector<glm::vec2> tex_coords = primitiva.get_texture_coordinates();

  vector<Shader> shaders;

  Shader first_object_shader(
      "engine/assets/shaders/vertex_from_buffers.glsl",
      "engine/assets/shaders/fragment_from_texture.glsl");

  Shader second_object_shader("engine/assets/shaders/vertex_from_buffers.glsl",
                              "engine/assets/shaders/fragment.glsl");

  Shader third_object_shader("engine/assets/shaders/vertex_from_buffers.glsl",
                             "engine/assets/shaders/fragment0.glsl");

  first_object_shader.option_command = SDLK_0;

  first_object_shader.active = true;
  second_object_shader.active = true;
  third_object_shader.active = true;

  // applies de mtl properties to the object shader
  if (mtl) {
    first_object_shader.Use();
    first_object_shader.Set("obj_ambient", material->obj_ambient);
    first_object_shader.Set("obj_diffuse", material->obj_diffuse);
    first_object_shader.Set("obj_specular", material->obj_specular);
  }
  tuple<Shader, int> firstShader = make_tuple(first_object_shader, SDLK_1);

  shaders.push_back(first_object_shader);
  shaders.push_back(second_object_shader);
  shaders.push_back(third_object_shader);

  glClearColor(0.5f, 0.7f, 0.2f, 1.0f);

  // BufferSet bufferSet = BufferSet(first_object_shader.program_id);

  // bufferSet.add(&vertices, "uPosition");
  // bufferSet.add(&indices);
  // bufferSet.add(&tex_coords, "tex_coords");
  // bufferSet.add(&normal, "normal");

  // -----------------------------------------------------------------------------------------------------//
  // Texture crap because we dont have a bind of textures

  // glBindVertexArray(bufferSet.getId());

  // for(Buffer b : bufferSet.buffers)
  //{
  //    GLint location = glGetAttribLocation(bufferSet.program,
  //    b.shader_var.c_str());
  //    glEnableVertexAttribArray(location);
  //    glBindBuffer(GL_ARRAY_BUFFER, 0);
  //}

  vector<tuple<TextureLoader, int, int>> textures;

  TextureLoader texture1("engine/assets/pixes.bmp");
  tuple<TextureLoader, int, int> firstTexture =
      make_tuple(texture1, 0, GL_TEXTURE0);

  textures.push_back(firstTexture);

  glBindVertexArray(0);

  // // Load the texture using any two methods
  // TextureLoader texture("../texture.jpg", 0, 0);

  // // Get a handle for our "myTextureSampler" uniform
  // GLuint TextureID  = glGetUniformLocation(first_object_shader.program_id,
  // "iChannel0");

  // // Bind our texture in Texture Unit 0
  // texture.Bind(0);
  // // Set our "myTextureSampler" sampler to use Texture Unit 0
  // glUniform1i(TextureID, 0);

  // vector<tuple<TextureLoader, int, int> > textures;

  // tuple<TextureLoader, int, int> firstTexture = make_tuple(texture, 0,
  // GL_TEXTURE0);

  // textures.push_back(firstTexture);

  // glBindVertexArray(0);
  // ------------------------------------------------------------

  // engine->Run(bufferSet.getId(), vertices.size());

  //   //---------------------------------------
  //   std::vector<float> vertices2 = vertices;

  //   std::transform(vertices2.begin(), vertices2.end(), vertices2.begin(),
  //                std::bind(std::multiplies<float>(), std::placeholders::_1,
  //                0.5));

  //   bufferSet.resize(&vertices2, "uPosition");
  // //---------------------------------------
  //
  // engine->Run(primitiva.get_buffer_set().getId(),
  // primitiva.get_indices_size(), shaders, textures);
  //
  // vector<GameObject*> primitivas;
  engine->Run(shaders, textures);

  return 0;
}
