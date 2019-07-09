#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include "shader.hpp"

#include "transformation.hpp"

using namespace std;
time_t tempo;
float xAxis;
float yAxis;
float xScale = 1.0f;
float yScale = 1.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
            glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            xScale+=0.1f;

        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
            glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            xScale-=0.1f;

        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
            glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            yScale+=0.1f;

        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
            glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            yScale-=0.1f;
    } else {
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            xAxis += 0.1f;
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            xAxis -= 0.1f;
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            yAxis += 0.1f;
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            yAxis -= 0.1f;    
    }

}

int main() {
    cout << "Game On" << endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Mac OS only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Playground", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int screenWidth = 800;
    int screenHeight = 600;
  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    //Assimp::Importer importer;
    //const aiScene *scene = importer.ReadFile("../X.obj", aiProcess_Triangulate);

    float vertices[] = {
       
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };
    float texCoords[] = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };
    unsigned int indices[] = {
        0, 1, 2
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VBO_textcoordinates;
    glGenBuffers(1, &VBO_textcoordinates);

    unsigned int EBO;
    glGenBuffers(1, &EBO);


    unsigned int TEX;
    glGenTextures(1, &TEX);
    stbi_set_flip_vertically_on_load(true); 
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../teste.jpg", &width, &height, &nrChannels, 0); 
    if (data)
    {
            glBindTexture(GL_TEXTURE_2D, TEX);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }   
    stbi_image_free(data);
    Shader shader("game/assets/shaders/vertex.glsl",
                  "game/assets/shaders/fragment.glsl");
    shader.use();



    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3*sizeof(float)));


    glBindBuffer(GL_ARRAY_BUFFER, VBO_textcoordinates);
    glBufferData(GL_ARRAY_BUFFER, sizeof texCoords, texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glActiveTexture(GL_TEXTURE0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);  

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);

    shader.use();

    while(!glfwWindowShouldClose(window))
    {
        tempo = time(0);

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // shader.use();
      //  glBindTexture(GL_TEXTURE_2D, TEX);
        // create transformations
         // make sure to initialize matrix to identity matrix first
        glm::mat4 transform = glm::mat4(1.0f);

        glm::vec2 cameraMove = glm::vec2(xAxis, yAxis);
        transform = camera(1.0f, cameraMove);

        // transform = translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = scale(transform, glm::vec3(xScale,yScale,1.0f));
        // std::cout <<  << std::endl;

        // get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(shader.program_id, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}