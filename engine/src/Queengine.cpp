#include "Queengine.h"
#include "Circle.hpp"
#include "Queengine.h"
#include "Rectangle.hpp"
#include "Triangle.hpp"
#include "transformation.hpp"
#include <cmath>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

Queengine *Queengine::instance = nullptr;

Queengine::Queengine() : primitives(0, 0) {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << "SDL_Init failed: %s\n" << SDL_GetError();
    exit(-1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  this->window = SDL_CreateWindow("Queengine", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, 0, 0,
                                  (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

  SDL_MaximizeWindow(this->window);

  if (this->window == nullptr) {
    std::cout << "Fail to create Window: " << SDL_GetError() << std::endl;
    exit(-1);
  }

  this->context = SDL_GL_CreateContext(this->window);

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }

  SDL_GetCurrentDisplayMode(0, &(this->currentDisplay));
  this->glCanvasArea = Rect(0, 0, currentDisplay.w, currentDisplay.h);
  glViewport(this->glCanvasArea.x, this->glCanvasArea.y, this->glCanvasArea.w,
             this->glCanvasArea.h);
}

Queengine::~Queengine() {
  SDL_GL_DeleteContext(this->context);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

Queengine *Queengine::GetInstance() {
  if (instance == nullptr) {
    instance = new Queengine();
  }

  return instance;
}

void ToggleFullscreen(SDL_Window *window) {
  bool isFullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN;
  SDL_SetWindowFullscreen(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
}

void BindUniforms(int &selected_primitive, glm::vec2 &last_mouse_position,
                  bool &press_primitive, int &vertice, GameObject *primitive,
                  Shader *shader,
                  vector<tuple<TextureLoader, int, int>> textures, float &xAxis,
                  float &yAxis, float &xScale, float &yScale) {
  glm::vec2 _ifFragCoordOffsetXY(0.0f, 0.0f);
  float _ifFragCoordScale = 1.0f;

  int width, height;
  SDL_DisplayMode currentDisplay;
  SDL_GetCurrentDisplayMode(0, &currentDisplay);
  width = currentDisplay.w;
  height = currentDisplay.h;

  glm::vec3 _resolution(1.0f, 1.0f, 0.5f);
  int mouseX = InputManager::GetInstance().GetMouseX();
  int mouseY = InputManager::GetInstance().GetMouseY();
  int _frame = 0;

  shader->Set("ifFragCoordOffsetUniform", 1, &_ifFragCoordOffsetXY.x);
  shader->Set("iResolution", 1, &_resolution.x);
  shader->Set("iTime", (float)(SDL_GetTicks() / 1000.0));
  shader->Set("iGlobalTime", (float)(SDL_GetTicks() / 1000.0));
  shader->Set("iMouse", mouseX * _resolution[0], mouseY * _resolution[0],
              mouseX * _resolution[0], mouseY * _resolution[0]);
  shader->Set("iFrame", &_frame);

  shader->Set("iChannel0", get<1>(textures[0]));
  // shader->Set("iChannel1", get<1>(textures[1]));
  // shader->Set("iChannel2", get<1>(textures[2]));
  // shader->Set("iChannel3", get<1>(textures[3]));

  // glm::mat4 rotation(glm::vec4(0.5, 0, -0.9, 0), glm::vec4(0, 1.0, 0, 0),
  //                   glm::vec4(0.9, 0, 0.5, 0), glm::vec4(0, 0, 0, 1));

  // glm::mat4 rotation2(glm::vec4(1, 0, 0, 0), glm::vec4(0, 0.5, -0.9, 0),
  //                    glm::vec4(0, 0.9, 0.5, 0), glm::vec4(0, 0, 0, 1));

  // shader->Set("rotation", rotation);
  // shader->Set("rotation2", rotation2);

  glm::mat4 Projection =
      glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

  shader->Set("Projection", Projection);

  glm::mat4 View = glm::lookAt(
      glm::vec3(0, 1.5, 3), // Camera is at (4,3,-3), in World Space
      glm::vec3(0, 0, 0),   // and looks at the origin
      glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
      );

  shader->Set("View", View);

  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 transform = glm::mat4(1.0f);

  vector<glm::vec3> f = primitive->get_coordinates();
  glm::vec3 mouse_position =
      glm::vec3(InputManager::GetInstance().GetMouseXCanvasCoord(),
                InputManager::GetInstance().GetMouseYCanvasCoord(), 0);
  shader->Set("Model", Model);

  if (InputManager::GetInstance().KeyPress(SDLK_1)) {
    selected_primitive = 0;
  } else if (InputManager::GetInstance().KeyPress(SDLK_2)) {
    selected_primitive = 1;
  } else if (InputManager::GetInstance().KeyPress(SDLK_3)) {
    selected_primitive = 2;
  } else if (InputManager::GetInstance().KeyPress(SDLK_4)) {
    selected_primitive = 3;
  } else if (InputManager::GetInstance().IsMouseDown(SDL_BUTTON_LEFT)) {
    vertice = primitive->get_vertice(mouse_position);
    if (vertice != -1) {
      f[vertice] =
          glm::vec3(InputManager::GetInstance().GetMouseXCanvasCoord(),
                    InputManager::GetInstance().GetMouseYCanvasCoord(), 0);
    }
  } else if (InputManager::GetInstance().MousePress(SDL_BUTTON_RIGHT)) {
    press_primitive = primitive->point_is_inside(mouse_position);
    if (press_primitive == true) {
      last_mouse_position =
          glm::vec2(InputManager::GetInstance().GetMouseXCanvasCoord(),
                    InputManager::GetInstance().GetMouseYCanvasCoord());
    }

  } else if (InputManager::GetInstance().IsMouseDown(SDL_BUTTON_RIGHT) &&
             press_primitive) {
    f = primitive->get_coordinates();
    glm::vec2 new_mouse_position(
        InputManager::GetInstance().GetMouseXCanvasCoord(),
        InputManager::GetInstance().GetMouseYCanvasCoord());

    for (int i = 0; i < f.size(); i++) {
      f[i].x -= last_mouse_position.x - new_mouse_position.x;
      f[i].y -= last_mouse_position.y - new_mouse_position.y;
    }

    last_mouse_position.x = new_mouse_position.x;
    last_mouse_position.y = new_mouse_position.y;
  }

  primitive->set_coordinates(f);

  // transform = camera(1.0f, cameraMove);

  // transform = rotate(transform, (float) (SDL_GetTicks()/1000.0),
  // glm::vec3(0.0f, 0.0f, 1.0f));
  // transform = scale(transform, glm::vec3(xScale, yScale, 1.0f));

  shader->Set("transform", transform);
}

// void Queengine::Run(unsigned int VAO, vector<Shader> shaderList,
// vector<tuple<TextureLoader, int, int> > textures) {
//  Queengine::Run(VAO, 3, shaderList, textures);
//}

void Queengine::Run(vector<Shader> &shaders,
                    vector<tuple<TextureLoader, int, int>> textures) {
  glEnable(GL_DEPTH_TEST);
  float xAxis = 0;
  float yAxis = 0;
  float xScale = 1.0f;
  float yScale = 1.0f;
  int vertice = -1;
  bool press_primitive = false;
  glm::vec2 last_mouse_position(0.0f, 0.0f);
  int selected_primitive = 0;

  while (not InputManager::GetInstance().QuitRequested()) {
    InputManager::GetInstance().Update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    InputManager::GetInstance().Update();

    this->HandleInput(shaders, textures);

    for (int i = 0; i < this->primitives.size(); i++) {
      shaders[i].Use();
      for (int j = 0; j < textures.size(); j++) {
        get<0>(textures[j]).Bind();
        glActiveTexture(get<2>(textures[j]));
      }
      std::cout << selected_primitive << std::endl;
      BindUniforms(selected_primitive, last_mouse_position, press_primitive,
                   vertice, (GameObject *)this->primitives[selected_primitive],
                   &shaders[i], textures, xAxis, yAxis, xScale, yScale);

      glBindVertexArray(
          ((GameObject *)this->primitives[i])->get_buffer_set().getId());

      glDrawElements(GL_TRIANGLES,
                     (((GameObject *)this->primitives[i])->get_indices_size()) /
                         sizeof(unsigned int),
                     GL_UNSIGNED_INT, 0);
    }

    SDL_GL_SwapWindow(this->window);
  }
}

Rect Queengine::GetGLCanvasArea() { return this->glCanvasArea; }

void Queengine::HandleInput(vector<Shader> &shaders,
                            vector<tuple<TextureLoader, int, int>> textures) {

  if (InputManager::GetInstance().KeyPress(F11_KEY)) {
    ToggleFullscreen(this->window);
  }

  if (InputManager::GetInstance().KeyPress(SDLK_t)) {
    Triangle *triangle = new Triangle(shaders[0]);
    this->primitives.push_back(triangle);
  }

  if (InputManager::GetInstance().KeyPress(SDLK_c)) {
    Circle *circle = new Circle(shaders[1], {0.0, 0.0, 0.0}, 0.5, 30);
    this->primitives.push_back(circle);
  }

  if (InputManager::GetInstance().KeyPress(SDLK_r)) {
    Rectangle *rectangle = new Rectangle(shaders[2]);
    this->primitives.push_back(rectangle);
  }
  // if (InputManager::GetInstance().KeyPress(SDLK_0)) {
  // for(int i = 0; i < shaderList.size(); i++){
  //}
  //}
}
