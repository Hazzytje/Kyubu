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
	
	float speed = 0.1f;
	this->vel.x = 0;
	this->vel.y = 0;
	
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->vel += Vector3(
			cos(camera.GetYaw()) * speed,
			sin(camera.GetYaw()) * speed,
			0//cos(camera.GetPitch()) * speed
		);
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->vel += Vector3(
			-cos(camera.GetYaw()) * speed,
			-sin(camera.GetYaw()) * speed,
			0//-cos(camera.GetPitch()) * speed
		);
	}
	//left/right motion
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->vel += Vector3(
			cos(float(camera.GetYaw() + (M_PI / 180) * -90)) * speed,
			sin(float(camera.GetYaw() + (M_PI / 180) * -90)) * speed,
			0
		);
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->vel += Vector3(
			cos(float(camera.GetYaw() + (M_PI / 180) * 90)) * speed,
			sin(float(camera.GetYaw() + (M_PI / 180) * 90)) * speed,
			0
		);
	}
	
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->vel.z = 0.25f;
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
	
	Entity::Update();
	
	camera.SetPos(this->pos.x, this->pos.y, this->pos.z + 1.7f);
}

void Player::Draw()
{
	
}