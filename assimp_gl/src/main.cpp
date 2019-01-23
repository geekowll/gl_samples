#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rendering/Shader.h"
#include "rendering/Texture.h"
#include "rendering/Model.h"

#include <thread>
#include <chrono>
#include <atomic>

GLFWwindow* window;
const int WINDOW_WIDTH  = 1024;
const int WINDOW_HEIGHT = 768;

Model   * mesh    = nullptr;
Shader  * shader  = nullptr;
Texture * texture = nullptr;

/* Matrices */
glm::vec3 cam_position = glm::vec3(0.0f, 1.0f, 1.2f);
glm::vec3 cam_look_at  = glm::vec3(0.0f, 0.5f, 0.0f);
glm::vec3 cam_up       = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 world_matrix      = glm::mat4(1.0f);
glm::mat4 view_matrix       = glm::lookAt(cam_position, cam_look_at, cam_up);
glm::mat4 projection_matrix = glm::perspectiveFov(glm::radians(60.0f), float(WINDOW_WIDTH), float(WINDOW_HEIGHT), 0.1f, 10.0f);

void window_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
  projection_matrix = glm::perspectiveFov(glm::radians(60.0f), float(width), float(height), 0.1f, 10.0f);
  
  if (shader != nullptr)
    {
      shader->setUniformMatrix4fv("viewProj", projection_matrix * view_matrix);
    }
}

int init()
{
  /* Initialize the library */
  if (!glfwInit())
    return -1;
  
  /* Create a windowed mode window and its OpenGL context */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello Modern GL!", nullptr, nullptr);
  
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  glfwSetWindowSizeCallback(window, window_size_callback);
  
  /* Initialize glad */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }
  
  /* Set the viewport */
  glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  
  glEnable(GL_DEPTH_TEST);
  
  return true;
}

int loadContent()
{
  mesh = new Model("res/models/alliance.obj");
  
  /* Create and apply basic shader */
  shader = new Shader("Basic.vert", "Basic.frag");
  shader->apply();
  
  shader->setUniformMatrix4fv("world",        world_matrix);
  shader->setUniformMatrix3fv("normalMatrix", glm::inverse(glm::transpose(glm::mat3(world_matrix))));
  shader->setUniformMatrix4fv("viewProj",     projection_matrix * view_matrix);
  
  shader->setUniform3fv("cam_pos", cam_position);
  
  texture = new Texture();
  texture->load("res/models/alliance.png");
  texture->bind();
  
  return true;
}

float startTime = static_cast<float>(glfwGetTime());

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* Update game time value */
  float newTime  = static_cast<float>(glfwGetTime());
  float gameTime = newTime - startTime;
  
  /* Draw our triangle */
  world_matrix = glm::rotate(glm::mat4(1.0f), gameTime * glm::radians(-90.0f), glm::vec3(0, 1, 0));
  
  shader->setUniformMatrix4fv("world", world_matrix);
  shader->setUniformMatrix3fv("normalMatrix", glm::inverse(glm::transpose(glm::mat3(world_matrix))));
  
  shader->apply();
  texture->bind();
  mesh->Draw();
}


std::atomic_bool first(true),second(true);

void update()
{
  std::cout << "in update" << std::endl;
  
  glfwMakeContextCurrent(window);
  
  /* Loop until the user closes the window */
  while (first && !glfwWindowShouldClose(window))
    {
      /* Render here */
      render();
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
    }
  
  glfwMakeContextCurrent(nullptr);
}

void update2(){
  std::cout << "in update 2" << std::endl;
  
  glfwMakeContextCurrent(window);
  
  /* Loop until the user closes the window */
  while (second && !glfwWindowShouldClose(window))
    {
      /* Render here */
      render();
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
    }
  
  glfwMakeContextCurrent(nullptr);
}

int main(void)
{
  if (!init())
    return -1;
  
  if (!loadContent())
    return -1;
  
  std::thread t(update);
  std::this_thread::sleep_for(std::chrono::seconds(5));
  first = false;
  
  std::thread t2(update2);
  std::this_thread::sleep_for(std::chrono::seconds(6));
  second = false;
  
  if(t.joinable())
    {
      t.join();
      t2.join();
    }
  
  std::cout << "in main" << std::endl;
  
  glfwMakeContextCurrent(window);
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      render();
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
    }
  
  glfwTerminate();
  
  delete mesh;
  delete shader;
  delete texture;
  
  return 0;
}
