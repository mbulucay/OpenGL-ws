#include "square.hpp"
#include <cstdlib>
#include <iostream>

#include <GLFW/glfw3.h>


Square::Square(const glm::vec2 center, float length)
    : m_center (center), m_length (length)
{ 

    m_pos = glm::vec3(m_center.x, m_center.y, 0.0f);

    float r = (10+rand()%25) / 40.0f;
    float g = (10+rand()%25) / 40.0f;
    float b = (10+rand()%25) / 40.0f;

    m_color = glm::vec4(r, g, b, 1.0f);
}


Square::~Square()
{ }


void Square::move(){

    if(m_dir == GLFW_KEY_UP && m_pos.y < 1.0f)
        m_pos.y += m_length;
    if(m_dir == GLFW_KEY_DOWN && m_pos.y > -1.0f)
        m_pos.y -= m_length;
    if(m_dir == GLFW_KEY_LEFT && m_pos.x > -1.0f)
        m_pos.x -= m_length;
    if(m_dir == GLFW_KEY_RIGHT && m_pos.x < 1.0f)
        m_pos.x += m_length;
}


