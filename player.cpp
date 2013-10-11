#include "player.h"
#include <GL/glfw3.h>
#include "globals.h"
#include <math.h>

Player::Player()
:entId(0), playerId(0), updatedPos(0), x(0), y(0), z(0)
{
	
}

Player::~Player()
{
	
}

void Player::Update()
{	
	GLFWwindow* window = Globals::getGameInstance().window;
	
	float oldX = camera.GetX(), oldY = camera.GetY(), oldZ = camera.GetZ();
	float speed = 1.0f;
	//forward/backward motion TODO: move to player class
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.AddX(sin(camera.GetPitch()) * cos(camera.GetYaw()) * speed);
		camera.AddY(sin(camera.GetPitch()) * sin(camera.GetYaw()) * speed);
		camera.AddZ(cos(camera.GetPitch()) * speed);
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.AddX(-sin(camera.GetPitch()) * cos(camera.GetYaw()) * speed);
		camera.AddY(-sin(camera.GetPitch()) * sin(camera.GetYaw()) * speed);
		camera.AddZ(-cos(camera.GetPitch()) * speed);
	}
	//left/right motion
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.AddX(cos(float(camera.GetYaw() + (M_PI / 180) * -90)) * speed);
		camera.AddY(sin(float(camera.GetYaw() + (M_PI / 180) * -90)) * speed);
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.AddX(cos(float(camera.GetYaw() + (M_PI / 180) * 90)) * speed);
		camera.AddY(sin(float(camera.GetYaw() + (M_PI / 180) * 90)) * speed);
	}
	Player::positionMutex.lock();
	Player::x = camera.GetX() - oldX;
	Player::y = camera.GetY() - oldY;
	Player::z = camera.GetZ() - oldZ;
	Player::updatedPos = true;
	Player::positionMutex.unlock();

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	double mouseDeltaX = mouseX - prevMouseX;
	double mouseDeltaY = mouseY - prevMouseY;

	float newYaw = float(mouseDeltaX) * 0.01f + camera.GetYaw();
	camera.SetYaw(newYaw);
	float newPitch = float(mouseDeltaY) * 0.01f + camera.GetPitch();
	if(newPitch > 0.01f && newPitch < (M_PI - 0.01f))
	{
		camera.SetPitch(newPitch);
	}

	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
	glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
}