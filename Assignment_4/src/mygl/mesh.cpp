#include "mesh.h"

Mesh meshCreate(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    GLuint vao = 0, vbo = 0, ebo = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glCheckError();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glCheckError();

        glEnableVertexAttribArray(eDataIdx::Position);
        glEnableVertexAttribArray(eDataIdx::Normal);
        glEnableVertexAttribArray(eDataIdx::UV);
        glVertexAttribPointer(eDataIdx::Position,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));
        glVertexAttribPointer(eDataIdx::Normal,     3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
        glVertexAttribPointer(eDataIdx::UV,         2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));
        glCheckError();
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return Mesh{vao, vbo, ebo, (unsigned int) vertices.size(), (unsigned int) indices.size()};
}

void meshDelete(const Mesh &mesh)
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ebo);
    glDeleteVertexArrays(1, &mesh.vao);
}
