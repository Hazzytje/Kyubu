#include "player.h"
#include <GL/glfw3.h>
#include "globals.h"
#include <math.h>

Player::Player()
:playerId(0)
{
	bbox = BoundingBox(-0.3, -0.3, 0.0, 0.3, 0.3, 1.8);
}

Player::~Player()
{
	
}

void Player::Update()
{	
	GLFWwindow* window = Globals::getGameInstance().window;
	
	Vector3 motion(0);
	
	float speed = 1.0f;
	//forward/backward motion TODO: move to player class
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		motion += Vector3(
			sin(camera.GetPitch()) * cos(camera.GetYaw()) * speed,
			sin(camera.GetPitch()) * sin(camera.GetYaw()) * speed,
			cos(camera.GetPitch()) * speed
		);
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		motion += Vector3(
			-sin(camera.GetPitch()) * cos(camera.GetYaw()) * speed,
			-sin(camera.GetPitch()) * sin(camera.GetYaw()) * speed,
			-cos(camera.GetPitch()) * speed
		);
	}
	//left/right motion
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		motion += Vector3(
			cos(float(camera.GetYaw() + (M_PI / 180) * -90)) * speed,
			sin(float(camera.GetYaw() + (M_PI / 180) * -90)) * speed,
			0
		);
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		motion += Vector3(
			cos(float(camera.GetYaw() + (M_PI / 180) * 90)) * speed,
			sin(float(camera.GetYaw() + (M_PI / 180) * 90)) * speed,
			0
		);
	}
	
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
	
	this->vel = motion;
	
	Entity::Update();
	
	camera.SetPos(this->pos.x, this->pos.y, this->pos.z);
}

void Player::Draw()
{
	
}