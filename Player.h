#pragma once
#include "CommonValues.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Player
{
public:
	Player();
	void setPlayerPosition(glm::vec3 position, GLfloat shiftX, GLfloat shiftY, GLfloat shiftZ);
	void setPlayerFront(glm::vec3 front, GLfloat shiftX, GLfloat shiftY, GLfloat shiftZ);
	void setPlayerFront(glm::vec3 front, GLfloat shiftAll);
	void setPlayerRotation(GLfloat playerYaw, GLfloat shiftYaw, GLfloat playerPitch, GLfloat shiftPitch);
	void keyControl(bool*keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange, GLfloat maxXChange, GLfloat maxYChange, GLfloat xInc, GLfloat yInc, GLfloat xDec, GLfloat yDec);

	glm::mat4 GetPlayerCalculatedPosition();
	~Player();
private:
	glm::mat4 mainModel;
	glm::vec3 playerPosition;
	glm::vec3 playerFront;
	void updateRot();
	void calculateRot();
	GLfloat yaw, pitch;
	GLfloat xrot, yrot;
};

