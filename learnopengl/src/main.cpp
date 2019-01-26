#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <chrono>
#include <atomic>

const int WINDOW_WIDTH  = 1024;
const int WINDOW_HEIGHT = 768;

void window_size_callback(GLFWwindow* window, int w, int h){
  glad_glViewport(0,0,w,h);
}

const char* vs_source = R"(#version 330 core
                        layout (location = 0) in vec3 aPos;
                        
                        void main()
                        {
                            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                        })";
const char* fs_source = R"(#version 330 core
                        out vec4 FragColor;
                        
                        void main()
                        {
                            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
                        })";
int main(){
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
  
  auto* window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"hello",NULL,NULL);
  if(window == NULL){
      std::cout << "failed to create window" << std::endl;
      glfwTerminate();
      return -1;
    }
  
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window,window_size_callback);
  
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
     std::cout << "glad error" << std::endl;
     glfwTerminate();
     return -1;
    }
  
  auto handleInput = [&]{
      if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
          glfwSetWindowShouldClose(window,true);
        }
    };
  
  GLuint vertex_shader;
  vertex_shader = glad_glCreateShader(GL_VERTEX_SHADER);
  
  glad_glShaderSource(vertex_shader,1,&vs_source,NULL);
  glad_glCompileShader(vertex_shader);
  
  int success;
  char infolog[512];
  glad_glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&success);
  if(!success){
      glad_glGetShaderInfoLog(vertex_shader,512,NULL,infolog);
      std::cout << "shader log:" << infolog << std::endl;
      return -1;
    }
  
  GLuint fragment_shader;
  fragment_shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
  glad_glShaderSource(fragment_shader,1,&fs_source,NULL);
  glad_glCompileShader(fragment_shader);
  glad_glGetShaderiv(fragment_shader,GL_COMPILE_STATUS,&success);
  if(!success){
      glad_glGetShaderInfoLog(fragment_shader,512,NULL,infolog);
      std::cout << "fragment:" << infolog << std::endl;
      return -1;
    }
  
  GLuint shader_program;
  shader_program = glad_glCreateProgram();
  glad_glAttachShader(shader_program,vertex_shader);
  glad_glAttachShader(shader_program,fragment_shader);
  glad_glLinkProgram(shader_program);
  glad_glGetProgramiv(shader_program,GL_LINK_STATUS,&success);
  if(!success){
      glad_glGetProgramInfoLog(shader_program,512,NULL,infolog);
      std::cout << "program:" << infolog << std::endl;
      return -1;
    }
  
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);  
  
  float vertices[] = {
       0.5f,  0.5f, 0.0f,  // top right
       0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.5f, 0.0f   // top left 
  };
  
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
      3, 2, 1    // second triangle
  };  
  
  
  GLuint vao,vbo,ebo;
  
  glad_glGenVertexArrays(1,&vao);
  glad_glGenBuffers(1,&vbo);
  glad_glGenBuffers(1,&ebo);
  
  glad_glBindVertexArray(vao);
  
  glad_glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STREAM_DRAW);
  
  glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
  glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
  
  glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
  glad_glEnableVertexAttribArray(0);
  
  glad_glBindBuffer(GL_ARRAY_BUFFER,0); // unbind vbo - attribpointer dedikten sorna gerek yok
  
  glad_glBindVertexArray(vao);        // vao bind edilmeden bu {} aktif edilemez.
  
  glad_glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  
  
  glad_glClearColor(0.1f,0.1f,0.1f,1.0f);
  
  while(!glfwWindowShouldClose(window)){
      handleInput();
      
      glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
      glad_glUseProgram(shader_program);
      glad_glBindVertexArray(vao);
      
      glad_glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
  
      glfwPollEvents();
      glfwSwapBuffers(window);
    }
  
  glDeleteVertexArrays(1,&vao);
  glDeleteBuffers(1,&vbo);
  glDeleteBuffers(1,&ebo);

  glfwTerminate();
  
  return 0;
}
