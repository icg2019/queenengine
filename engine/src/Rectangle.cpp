#include "../include/Rectangle.hpp"
#include "../include/Triangle.hpp"

Rectangle::Rectangle(Shader shader) : upper_triangle(shader), lower_triangle(shader){
    // Create two triangles and set the attributes to them
    std::vector<glm::vec2> upper_triangle_texture_coordinates = {
		glm::vec2(0.0, 0.0),
		glm::vec2(1.0, 0.0),
		glm::vec2(1.0, 1.0)
	};

    std::vector<glm::vec3> upper_triangle_coordinates = {
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(1.0, 0.0, 0.0),
		glm::vec3(1.0, 1.0, 0.0)
	};

    // upper_triangle.set_texture_coordinates(upper_triangle_texture_coordinates);
    upper_triangle.set_coordinates(upper_triangle_coordinates);
}

Rectangle::Rectangle(Shader shader, std::vector<glm::vec3> coordinates) : upper_triangle(shader), lower_triangle(shader){
    if(coordinates.size() != 4){
        throw "Rectangle must contain 4 coordinates";
    }

    std::vector<glm::vec3> coordinates_upper_triangle = {};
    std::vector<glm::vec1> indices_upper_triangle = {};

    std::vector<glm::vec3> coordinates_lower_triangle = {};
    std::vector<glm::vec1> indices_lower_triangle = {};

    for(int i = 0; i < coordinates.size(); i++){
        if(i < 3) { // coordinates for upper triangle
            coordinates_upper_triangle.push_back(coordinates[i]);
        }
        if(i > 0) { // coordinates for lower triangle
            coordinates_lower_triangle.push_back(coordinates[i]);
        }
	}

    for(int i = 0; i < 3; i++){
        indices_upper_triangle.push_back(glm::vec1(i));
        indices_lower_triangle.push_back(glm::vec1(i+3));
    }

    this->upper_triangle = Triangle(shader, coordinates_upper_triangle, indices_upper_triangle);
    this->lower_triangle = Triangle(shader, coordinates_lower_triangle, indices_lower_triangle);

    std::vector<glm::vec2> upper_triangle_texture_coordinates = {
		glm::vec2(0.0, 0.0),
		glm::vec2(1.0, 0.0),
		glm::vec2(1.0, 1.0),
	};

    upper_triangle.set_texture_coordinates(upper_triangle_texture_coordinates);
}

std::vector<float> Rectangle::get_coordinates(){
    int number_of_values = 18; // for each triangle there are 9 float values. So 9 * 2 (triangles) = 18
    std::vector<float> float_array = std::vector<float>(number_of_values);
    std::vector<float> array_upper_triangle = this->upper_triangle.get_coordinates();
    std::vector<float> array_lower_triangle = this->lower_triangle.get_coordinates();

    // memcpy(float_array, array_upper_triangle, sizeof(float) * 9);
    // memcpy(float_array + (sizeof(float) * 9), array_low(unsigned int*) malloc(sizeof(unsigned int) * number_of_indices);er_triangle, sizeof(float) * 9);

    for(size_t i = 0; i < 9; i++){
        float_array[i] = array_upper_triangle[i];
        float_array[i+9] = array_lower_triangle[i];
    }

    return float_array;
}

void Rectangle::set_coordinates(std::vector<glm::vec3>){
    // TODO
}

unsigned int Rectangle::get_coordinates_size(){
    // Return size of the combined array
    return this->upper_triangle.get_coordinates_size() + this->lower_triangle.get_coordinates_size();
}

std::vector<unsigned int> Rectangle::get_indices(){
    // Combines the two indeces arrays from the two triangles and return them as one
    int number_of_indices = 6; // Each point of both triangles has an indice
    std::vector<unsigned int> indices_array = std::vector<unsigned int>(number_of_indices);
    std::vector<unsigned int> indices_upper_triangle = this->upper_triangle.get_indices();
    std::vector<unsigned int> indices_lower_triangle = this->lower_triangle.get_indices();

    // memcpy(indices_array, indices_upper_triangle, sizeof(unsigned int) * 3);
    // memcpy(indices_array + (sizeof(unsigned int) * 3), indices_lower_triangle, sizeof(unsigned int) * 3);

    for(size_t i = 0; i < 3; i++){
        indices_array[i] = indices_upper_triangle[i];
        indices_array[i+3] = indices_lower_triangle[i];
    }

    return indices_array;
}

void Rectangle::set_indices(std::vector<glm::vec1>){
    // TODO
}

unsigned int Rectangle::get_indices_size(){
    // Returns size of the combined index array
    return this->upper_triangle.get_indices_size() + this->lower_triangle.get_indices_size();
}

std::vector<glm::vec2> Rectangle::get_texture_coordinates(){
    int number_of_indices = 6; // Each point of both triangles has an indice
    std::vector<glm::vec2> text_coord = std::vector<glm::vec2>(number_of_indices);
    std::vector<glm::vec2> text_coords_upper_triangle = this->upper_triangle.get_texture_coordinates();
    std::vector<glm::vec2> text_coords_lower_triangle = this->lower_triangle.get_texture_coordinates();

    for(size_t i = 0; i < 3; i++){
        text_coord[i] = text_coords_upper_triangle[i];
        text_coord[i+3] = text_coords_lower_triangle[i];
    }

    return text_coord;
}