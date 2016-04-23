#include "particlescenenode.h"

ParticleSceneNode::ParticleSceneNode()
    : m_program(new QOpenGLShaderProgram)
    , m_square_buffer(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , m_particle_verts_buffer(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
{
    GLfloat square_verts[] = {-.1, -.1, 0.0,
                              -.1,  .1, 0.0,
                               .1,  .1, 0.0,
                               .1, -.1, 0.0};

    GLfloat particle_verts[] = {-.5f, -.5f,  0.0f,
                                 .5f, -.5f,  0.0f,
                                 .5f,  .5f,  0.0f,
                                -.5f,  .5f,  0.0f,
                                -.5f, -.5f,  0.5f,
                                 .5f, -.5f,  0.5f,
                                 .5f,  .5f,  0.5f,
                                -.5f,  .5f,  0.5f,};
    m_particle_verts_count = sizeof(particle_verts)/sizeof(particle_verts[0])/3;

    //Initialize buffers

    m_vao.create();
    m_vao.bind();

    m_square_buffer.create();
    m_square_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_square_buffer.bind();
    m_square_buffer.allocate(square_verts, sizeof(square_verts));
    m_square_buffer.release();

    m_particle_verts_buffer.create();
    m_particle_verts_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_particle_verts_buffer.bind();
    m_particle_verts_buffer.allocate(particle_verts, sizeof(particle_verts));
    m_particle_verts_buffer.release();

    m_square_buffer.release();

    m_vao.release();

    //initialize shader program

    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                       ":/shaders/particle1.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                       ":/shaders/particle1.fsh");
    m_program->link();

}

ParticleSceneNode::~ParticleSceneNode()
{
}

void ParticleSceneNode::draw_this(QOpenGLFunctions *func, glm::mat4 matrix)
{
    m_program->bind();
    int mvpID = m_program->uniformLocation("mvp");
    func->glUniformMatrix4fv(mvpID, 1, GL_FALSE, &matrix[0][0]);

    //draw particle buffers
    m_vao.bind();

    int vertsID = m_program->attributeLocation("verts");
    m_particle_verts_buffer.bind();

    m_program->enableAttributeArray(vertsID);
    m_program->setAttributeBuffer(vertsID, GL_FLOAT, 0, 3, 0);

    func->glDrawArrays(GL_POINTS, 0, m_particle_verts_count);
    m_particle_verts_buffer.release();

    m_program->disableAttributeArray(vertsID);

    m_vao.release();
    m_program->release();
}

void ParticleSceneNode::update_this(UpdateContainer updates)
{
    //update particle system state
    //fetch particle system state and feed buffers
}
