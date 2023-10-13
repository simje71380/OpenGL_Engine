#include "Camera.h"

void Camera::Resize(int width, int height)
{
	m_Width = (float)width;
	m_Height = (float)height;
}

glm::mat4 Camera::Get_MVP(glm::mat4 model)
{
	// Projection matrix : Field of View, aspect ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(m_FOV), m_Width / m_Height, m_Range_Close, m_Range_Far);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		m_Position, // Camera position in World Space
		m_Position + m_Orientation, // and looking position
		m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0) // Head is up (set to 0,-1,0 to look upside-down)
	);
	return Projection * View * model;
}

glm::mat4 Camera::Get_View()
{
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		m_Position, // Camera position in World Space
		m_Position + m_Orientation, // and looking position
		m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0) // Head is up (set to 0,-1,0 to look upside-down)
	);
	return View;
}

glm::mat4 Camera::Get_Proj()
{
	// Projection matrix : Field of View, aspect ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(m_FOV), m_Width / m_Height, m_Range_Close, m_Range_Far);

	return Projection;
}


void Camera::Inputs(GLFWwindow* window, double deltaTime)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Position += (float)deltaTime * m_speed * m_Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position += (float)deltaTime * m_speed * -glm::normalize(glm::cross(m_Orientation, m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0)));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Position += (float)deltaTime * m_speed * -m_Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += (float)deltaTime * m_speed * glm::normalize(glm::cross(m_Orientation, m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0)));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_Position += (float)deltaTime * m_speed * glm::normalize(m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_Position += (float)deltaTime * m_speed * glm::normalize(m_Head_Is_Up ? glm::vec3(0, -1, 0) : glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_speed = 40.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		m_speed = 10.0f;
	}

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (m_firstClick)
		{
			glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
			m_firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = m_sensitivity * (float)(mouseY - (m_Height / 2)) / m_Height;
		float rotY = m_sensitivity * (float)(mouseX - (m_Width / 2)) / m_Width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0))));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0)) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Head_Is_Up ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0));

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		m_firstClick = true;
	}
}