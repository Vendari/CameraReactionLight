#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);

	lightProj = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.5f, 100.0f);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

void DirectionalLight::SetLightDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::SetLightColor(GLfloat redP, GLfloat greenP, GLfloat blueP)
{
	Light::colour = glm::vec3(redP, greenP, blueP);
}

void DirectionalLight::SetLightIntense(GLfloat aIntensityP, GLfloat dIntensityP)
{
	Light::ambientIntensity = aIntensityP;
	Light::diffuseIntensity = dIntensityP;
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
}
