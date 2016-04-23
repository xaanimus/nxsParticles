#ifndef SCENENODE_H
#define SCENENODE_H

#include <QtGui>
#include <vector>
#include "glm/glm.hpp"
#include "utilities.h"

class SceneNode
{
public:
    SceneNode();

    void draw(QOpenGLFunctions * func, glm::mat4 matrix);
    void update(UpdateContainer updates);
    void add_child(SceneNode &child);

private:
    virtual void draw_this(QOpenGLFunctions * func, glm::mat4 matrix) = 0;
    virtual void update_this(UpdateContainer updates) = 0;

    glm::vec3 m_pos;
    glm::vec3 m_rot;
    glm::vec3 m_scale;

    std::vector<SceneNode> m_children;
};


class GroupSceneNode : public SceneNode
{
public:
private:
    virtual void draw_this(QOpenGLFunctions * func, glm::mat4 matrix);
    virtual void update_this(UpdateContainer updates);
};


class TriangleSceneNode : public SceneNode
{
public:
    TriangleSceneNode();
private:
    virtual void draw_this(QOpenGLFunctions * func, glm::mat4 matrix);
    virtual void update_this(UpdateContainer updates);

    int m_verts_count, m_element_count;

    QOpenGLBuffer m_verts_buffer;
    QOpenGLBuffer m_element_buffer;
    QOpenGLVertexArrayObject m_vao;

    std::vector<GLfloat> m_verts;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
};

#endif // SCENENODE_H
