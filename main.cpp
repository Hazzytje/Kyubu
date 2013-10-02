#include <GL/glew.h>
#include <stdio.h>
#include <GL/glfw3.h>
#include <btBulletDynamicsCommon.h>
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

namespace Globals
{
	Camera camera;
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
	ChunkHandler chunkhandler;
	glClearColor(0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 0.0f);

	double prevMouseX, prevMouseY;

	glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
	glEnable(GL_DEPTH_TEST);

	NetworkHandler bla;

	while(!glfwWindowShouldClose(window))
	{
		chunkhandler.Update();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float oldX = Globals::camera.GetX(), oldY = Globals::camera.GetY(), oldZ = Globals::camera.GetZ();
		float speed = 1.0f;
		//forward/backward motion TODO: move to player class
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Globals::camera.AddX(sin(Globals::camera.GetPitch()) * cos(Globals::camera.GetYaw()) * speed);
			Globals::camera.AddY(sin(Globals::camera.GetPitch()) * sin(Globals::camera.GetYaw()) * speed);
			Globals::camera.AddZ(cos(Globals::camera.GetPitch()) * speed);
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Globals::camera.AddX(-sin(Globals::camera.GetPitch()) * cos(Globals::camera.GetYaw()) * speed);
			Globals::camera.AddY(-sin(Globals::camera.GetPitch()) * sin(Globals::camera.GetYaw()) * speed);
			Globals::camera.AddZ(-cos(Globals::camera.GetPitch()) * speed);
		}
		//left/right motion
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Globals::camera.AddX(cos(float(Globals::camera.GetYaw() + (M_PI / 180) * -90)) * speed);
			Globals::camera.AddY(sin(float(Globals::camera.GetYaw() + (M_PI / 180) * -90)) * speed);
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Globals::camera.AddX(cos(float(Globals::camera.GetYaw() + (M_PI / 180) * 90)) * speed);
			Globals::camera.AddY(sin(float(Globals::camera.GetYaw() + (M_PI / 180) * 90)) * speed);
		}
		Player::positionMutex.lock();
		Player::x = Globals::camera.GetX() - oldX;
		Player::y = Globals::camera.GetY() - oldY;
		Player::z = Globals::camera.GetZ() - oldZ;
		Player::updatedPos = true;
		Player::positionMutex.unlock();

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		double mouseDeltaX = mouseX - prevMouseX;
		double mouseDeltaY = mouseY - prevMouseY;

		float newYaw = float(mouseDeltaX) * 0.01f + Globals::camera.GetYaw();
		Globals::camera.SetYaw(newYaw);
		float newPitch = float(mouseDeltaY) * 0.01f + Globals::camera.GetPitch();
		if(newPitch > 0.01f && newPitch < (M_PI - 0.01f))
		{
			Globals::camera.SetPitch(newPitch);
		}

		glfwSetCursorPos(window, 1024 / 2, 768 / 2);
		glfwGetCursorPos(window, &prevMouseX, &prevMouseY);

		//Globals::PrintAllGlErrors();
		//printf("at file %s, line %i\n", __FILE__, __LINE__);
		chunkhandler.Render();
		//Globals::PrintAllGlErrors();
		//printf("at file %s, line %i\n", __FILE__, __LINE__);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Globals::PrintAllGlErrors();
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

