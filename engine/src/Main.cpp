#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <functional>

#include <glad/glad.h>
#include <glm/glm.hpp>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

#define INCLUDE_SDL
#include <glm/gtc/matrix_transform.hpp>

#include "SDL_include.h"
#include "InputManager.h"
// #include "Texture.h"
#include "Queengine.h"
#include "BufferSet.hpp"
#include "Triangle.hpp"
#include "Rectangle.hpp"
#include "Circle.hpp"
#include "Shader.h"
// #include "TextureLoader.h"
#include "Model.hpp"

#include "../include/log.h"

using namespace std;

int main(int argc, char **argv) {
  Queengine *engine = Queengine::GetInstance();

  // std::vector<float> rotation = {
  //     0.5, 0, -0.9, 0,
  //     0, 1.0, 0, 0,
  //     0.9, 0, 0.5, 0,
  //     0, 0, 0, 1
  // };

  // std::vector<float> rotation2 = {
  //     1, 0, 0, 0,
  //     0, 0.5, -0.9, 0,
  //     0, 0.9, 0.5, 0,
  //     0, 0, 0, 1
  // };
  
  vector<glm::vec3> my_coordinates = {
		glm::vec3(-0.5, 0.5, 0.0),
		glm::vec3(0.5, 0.5, 0.0),
		glm::vec3(-0.5, -0.5, 0.0),
    glm::vec3(0.5, -0.5, 0.0),
	};


  Shader first_object_shader("engine/assets/shaders/vertex_from_buffers.glsl",
                "engine/assets/shaders/fragment_from_buffers.glsl");

  // Triangle primitiva = Triangle(first_object_shader);
  // Rectangle primitiva = Rectangle(first_object_shader, my_coordinates);
  // Circle primitiva = Circle(first_object_shader, {0.0, 0.0, 0.0}, 0.5, 30);

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec3> normal;
  std::vector<unsigned int> indices;
  std::vector<float> light = {1, 0, 0};

  bool fileOpened = load3DOBJ("engine/assets/obj/suzanne.obj", vertices, tex_coords, normal);

  for(int i = 0; i < vertices.size();i++) {
    indices.push_back(i);
  }

  vector<tuple<Shader, int>> shaders;

  first_object_shader.active = false;
  tuple<Shader, int> firstShader = make_tuple(first_object_shader, SDLK_1);

  shaders.push_back(firstShader);


  INFO("Initializing VAO");
  glClearColor(0.5f, 0.7f, 0.2f, 1.0f);
  // -----------------------------------------------------------------------------------------------------//
  // Texture crap because we dont have a bind of textures

  // BufferSet bufferSet = primitiva.get_buffer_set();

  BufferSet bufferSet = BufferSet(first_object_shader.program_id);
  
  bufferSet.add(&vertices, "uPosition");
  
  bufferSet.add(&indices);

  bufferSet.add(&tex_coords,"tex_coords");

  glBindVertexArray(bufferSet.getId());

  for(Buffer b : bufferSet.buffers)
  {
      GLint location = glGetAttribLocation(bufferSet.program, b.shader_var.c_str());
      glEnableVertexAttribArray(location);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  vector<tuple<TextureLoader, int, int> > textures;

  TextureLoader texture1("engine/assets/pixes.bmp");
  tuple<TextureLoader, int, int> firstTexture = make_tuple(texture1, 0, GL_TEXTURE0);


  textures.push_back(firstTexture);

  glBindVertexArray(0);

  // // Load the texture using any two methods
  // TextureLoader texture("../texture.jpg", 0, 0);

	// // Get a handle for our "myTextureSampler" uniform
	// GLuint TextureID  = glGetUniformLocation(first_object_shader.program_id, "iChannel0");

  // // Bind our texture in Texture Unit 0
  // texture.Bind(0);
  // // Set our "myTextureSampler" sampler to use Texture Unit 0
  // glUniform1i(TextureID, 0);

  // vector<tuple<TextureLoader, int, int> > textures;

  // tuple<TextureLoader, int, int> firstTexture = make_tuple(texture, 0, GL_TEXTURE0);

  // textures.push_back(firstTexture);

  // glBindVertexArray(0);
  // ------------------------------------------------------------


  // engine->Run(bufferSet.getId(), vertices.size());

//   //---------------------------------------
//   std::vector<float> vertices2 = vertices;
//   std::transform(vertices2.begin(), vertices2.end(), vertices2.begin(),
//                std::bind(std::multiplies<float>(), std::placeholders::_1, 0.5));


//   bufferSet.resize(&vertices2, "uPosition");
// //---------------------------------------
  engine->Run(bufferSet.getId(), indices.size(), shaders, textures);

  return 0;
}
