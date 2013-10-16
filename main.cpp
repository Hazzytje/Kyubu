#include <GL/glew.h>
#include <stdio.h>
#include <GL/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "player.h"
#include "blocks.h"
#include "chunkHandler.h"
#include "globals.h"
#include "textureloader.h"
#include "networkhandler.h"


void keyCallback(GLFWwindow* window, int key, int action)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main(int argc, char* argv[])
{
	Blocks::initBlocks();
	if(glfwInit() != GL_TRUE)
	{
		printf("glfwInit failed!\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Kyubu", NULL, NULL);

	if(!window)
	{
		printf("glfwCreateWindow failed!\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		printf("glewInit failed!\n");
		printf("Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
		return -1;
	}
	GLenum currentError = glGetError();
	if(!(currentError == GL_NO_ERROR || currentError == GL_INVALID_ENUM))
	{
		printf("something's wrong with glew...%x\n", currentError);
	}

	glfwSetKeyCallback(window, keyCallback);

	glClearColor(0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 0.0f);

	Globals::gameInstance = new Game(window);

	glEnable(GL_DEPTH_TEST);
	
	while(!glfwWindowShouldClose(window))
	{
		Globals::getGameInstance().Update();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Globals::getGameInstance().Draw();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Globals::PrintAllGlErrors();
	delete Globals::gameInstance;
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

