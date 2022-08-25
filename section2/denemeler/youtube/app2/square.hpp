#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


class Square{

    public:

        Square(const glm::vec2 center, float length);
        ~Square();

        float getLength() const { return m_length; }
        glm::vec2 getCenter() const { return m_center; }
        glm::vec3 getPos() const { return m_pos; }
        glm::vec4 getColor() const { return m_color; }
        int getDir() const { return m_dir; }
        int setDir(int dir) { return m_dir = dir; }

        void move();

    private:

        glm::vec2 m_center;
        float m_length;
        int m_dir;

        glm::vec3 m_pos;
        glm::vec4 m_color;

};

#endif