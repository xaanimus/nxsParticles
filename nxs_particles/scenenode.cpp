#include "scenenode.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

SceneNode::SceneNode()
    : m_scale(glm::vec3(1.0,1.0,1.0))
{}

void SceneNode::draw(QOpenGLFunctions *func, glm::mat4 matrix)
{
    matrix = glm::scale(matrix, m_scale);
    matrix = glm::translate(matrix, m_pos);
    matrix = matrix * glm::orientate4(m_rot);

    draw_this(func, matrix);

    for(SceneNode &child : m_children) {
        child.draw(func, matrix);
    }
}

void SceneNode::update(UpdateContainer updates)
{
    update_this(updates);

    for (SceneNode &child : m_children) {
        child.update(updates);
    }
}

void SceneNode::add_child(SceneNode &child)
{
}

void GroupSceneNode::draw_this(QOpenGLFunctions*, glm::mat4)
{}
void GroupSceneNode::update_this(UpdateContainer)
{}

TriangleSceneNode::TriangleSceneNode()
    : m_verts_buffer(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , m_element_buffer(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
{
    GLfloat verts[] = {-.5f, -.5f, 0.f,
                       .5f, -.5f, 0.f,
                        0.f, .5f, 0.f};
    m_verts_count = sizeof(verts)/sizeof(verts[0]);

    GLuint elems[] = {0,1,2};
    m_element_count = sizeof(elems)/sizeof(elems[0]);

    m_vao.create();
    m_vao.bind();

    m_verts_buffer.create();
    m_verts_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_verts_buffer.bind();
    m_verts_buffer.allocate(verts, sizeof(verts));
    m_verts_buffer.release();

    m_element_buffer.create();
    m_element_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_element_buffer.bind();
    m_element_buffer.allocate(elems, sizeof(elems));
    m_element_buffer.release();

    m_vao.release();

    m_verts = {-.5, -.5, 0,
               0, .5, 0,
               .5, -.5, 0};

    m_program = std::unique_ptr<QOpenGLShaderProgram>(new QOpenGLShaderProgram());

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                     "attribute highp vec3 verts;\n"
                                     "varying highp vec4 color;\n"
                                     "uniform highp mat4 mvp;\n"
                                     "void main() {"
                                     "color = vec4(verts + vec3(.5,.5,1.0), 1.0);"
                                     "gl_Position = mvp * vec4(verts, 1.0);"
                                     "}\n");
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     "varying highp vec4 color;\n"
                                     "void main() {"
                                     "gl_FragColor = vec4(1.0,1.0,1.0,1.0);"
                                     "}\n");
    m_program->link();
}

//TODO Debug mvp!
void TriangleSceneNode::draw_this(QOpenGLFunctions *func, glm::mat4 matrix)
{

    m_program->bind();
    m_vao.bind();

    int vertsID = m_program->attributeLocation("verts");
    int mvpID = m_program->uniformLocation("mvp");
    func->glUniformMatrix4fv(mvpID, 1, GL_FALSE, &matrix[0][0]);

    m_verts_buffer.bind();
    m_program->enableAttributeArray(vertsID);
    m_program->setAttributeBuffer(vertsID, GL_FLOAT, 0, 3, 0);
    m_verts_buffer.release();

    m_element_buffer.bind();
    func->glDrawElements(GL_TRIANGLES, m_element_count, GL_UNSIGNED_INT, 0);
    m_element_buffer.release();

    m_vao.release();
    m_program->release();

    /*
    m_program->bind();
    int vertsID = m_program->attributeLocation("verts");

    m_program->enableAttributeArray(vertsID);
    m_program->setAttributeArray(vertsID, GL_FLOAT, m_verts.data(), 3);

    func->glDrawArrays(GL_TRIANGLES, 0, 3);

    m_program->disableAttributeArray(vertsID);
    m_program->release();
    */
}

void TriangleSceneNode::update_this(UpdateContainer updates)
{}
