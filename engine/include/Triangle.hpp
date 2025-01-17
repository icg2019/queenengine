#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

// #include <bits/stdc++.h>
#include "BufferSet.hpp"
#include "GameObject.hpp"
#include "Shader.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/vec1.hpp>
#include <iostream>
#include <string>
#include <vector>

class Triangle : public GameObject {
private:
  std::vector<glm::vec3> coordinates;
  std::vector<glm::vec2> texture_coordinates;
  std::string texture_path;
  std::vector<glm::vec1> indices;
  BufferSet bufferSet;

public:
  Triangle(Shader shader);
  Triangle(Shader shader, std::vector<glm::vec3> coordinates,
           std::vector<glm::vec1> indices);

  std::vector<glm::vec3> get_coordinates();
  void set_coordinates(std::vector<glm::vec3>);

  unsigned int get_coordinates_size();

  std::vector<glm::vec2> get_texture_coordinates();
  void set_texture_coordinates(std::vector<glm::vec2>);

  std::string get_texture_path();
  void set_texture_path(const std::string &);

  std::vector<unsigned int> get_indices();
  void set_indices(std::vector<glm::vec1>);

  unsigned int get_indices_size();

  bool is_a_valid_path(std::string &);

  BufferSet get_buffer_set();
  int get_vertice(glm::vec3);
  bool point_is_inside(glm::vec3);

  // Calcular a normal da face do triangulo
};

#endif
