#include "Player.h"



Player::Player()
{
}

void Player::setPlayerPosition(glm::vec3 position, GLfloat shiftX, GLfloat shiftY, GLfloat shiftZ)
{
	mainModel = glm::mat4();
	playerPosition = glm::vec3(position.x + shiftX, position.y + shiftY, position.z + shiftZ);
}

void Player::setPlayerFront(glm::vec3 front, GLfloat shiftX, GLfloat shiftY, GLfloat shiftZ)
{
	playerFront = glm::vec3(front.x , front.y , front.z)*glm::vec3(shiftX, shiftY, shiftZ);
}

void Player::setPlayerFront(glm::vec3 front, GLfloat shiftAll)
{
	playerFront = glm::vec3(front.x, front.y, front.z)*glm::vec3(shiftAll, shiftAll, shiftAll);
}

void Player::setPlayerRotation(GLfloat playerYaw, GLfloat shiftYaw, GLfloat playerPitch, GLfloat shiftPitch)
{
	yaw = playerYaw + shiftYaw;
	pitch = playerPitch + shiftPitch;
}

void Player::keyControl(bool * keys, GLfloat deltaTime)
{
}

void Player::mouseControl(GLfloat xChange, GLfloat yChange, GLfloat maxXChange, GLfloat maxYChange, GLfloat xInc, GLfloat yInc, GLfloat xDec, GLfloat yDec)
{
	if (xChange > 0)	xrot += xChange * xInc;
	else if (xChange < 0)	xrot += xChange * xInc;
	if (xrot > maxXChange) xrot = maxXChange;
	else if (xrot < -maxXChange) xrot = -maxXChange;

	if (yChange > 0)	yrot += yChange * yInc;
	else if (yChange < 0)	yrot += yChange * yInc;
	if (yrot > maxYChange) yrot = maxYChange;
	else if (yrot < -maxYChange) yrot = -maxYChange;

	if (xrot > xDec)
	{
		xrot -= xDec;
	}
	else if (xrot < 0) {
		xrot += xDec;
	}

	if (yrot > yDec)
	{
		yrot -= yDec;
	}
	else if (yrot < 0) {
		yrot += yDec;
	}
}


glm::mat4 Player::GetPlayerCalculatedPosition()
{
	mainModel = glm::translate(mainModel, playerPosition+playerFront);
	calculateRot();
	updateRot();
	return mainModel;
}

void Player::updateRot()
{
	mainModel = glm::rotate(mainModel, xrot*TO_RADIANS, glm::vec3(0.0f, 0.0f, 1.0f));
	mainModel = glm::rotate(mainModel, yrot*TO_RADIANS, glm::vec3(-1.0f, 0.0f, 0.0f));
}

void Player::calculateRot()
{
	mainModel = glm::rotate(mainModel, (yaw)*TO_RADIANS, glm::vec3(0.0f, -1.0f, 0.0f));
	mainModel = glm::rotate(mainModel, (pitch)*TO_RADIANS, glm::vec3(-1.0f, 0.0f, 0.0f));
}

Player::~Player()
{
}


