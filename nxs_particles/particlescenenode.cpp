#include "particlescenenode.h"

#include "texturegenerator.h"

ParticleSceneNode::ParticleSceneNode()
    : m_program(new QOpenGLShaderProgram)
    , m_square_buffer(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , m_particle_verts_buffer(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , m_tex_uv_buffer(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , m_particle_system(new ParticleSystem)
{
    //vertex data
    GLfloat square_verts[] = {-.1, -.1, 0.0,
                              -.1,  .1, 0.0,
                               .1, -.1, 0.0,
                               .1,  .1, 0.0};

    GLfloat particle_verts[] = {-.5f, -.5f,  0.0f,
                                 .5f, -.5f,  0.0f,
                                 .5f,  .5f,  0.0f,
                                -.5f,  .5f,  0.0f,
                                -.5f, -.5f,  0.5f,
                                 .5f, -.5f,  0.5f,
                                 .5f,  .5f,  0.5f,
                                -.5f,  .5f,  0.5f,};

    GLfloat tex_uv[] = {0.0f, 1.0f,
                        0.0f, 0.0f,
                        1.0f, 1.0f,
                        1.0f, 0.0f};

    m_particle_verts_count = sizeof(particle_verts)/sizeof(particle_verts[0])/3;
    m_billboard_verts_count = sizeof(square_verts)/sizeof(square_verts[0])/3;
    m_tex_uv_count = sizeof(tex_uv)/sizeof(tex_uv)/3;


    //Texture
    std::vector<uint8_t> &particle_texture = tex_gen::particle_texture_1();
    int tex_size = tex_gen::particle_tex_1_size();

    qDebug() << "tx" << particle_texture[100];

    //glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 tex_size, //+2
                 tex_size, //+2
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 particle_texture.data());

    //Initialize buffers

    m_vao.create();
    m_vao.bind();

    m_square_buffer.create();
    m_square_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_square_buffer.bind();
    m_square_buffer.allocate(square_verts, sizeof(square_verts));
    m_square_buffer.release();

    m_particle_verts_buffer.create();
    m_particle_verts_buffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_particle_verts_buffer.bind();
    m_particle_verts_buffer.allocate(particle_verts, sizeof(particle_verts));
    m_particle_verts_buffer.release();

    m_tex_uv_buffer.create();
    m_tex_uv_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_tex_uv_buffer.bind();
    m_tex_uv_buffer.allocate(tex_uv, sizeof(tex_uv));
    m_tex_uv_buffer.release();

    m_vao.release();

    //initialize shader program

    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                       ":/shaders/particle1.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                       ":/shaders/particle1.fsh");
    m_program->link();

    m_loc_stride = 0;
    m_loc_offset = 0;
    m_loc_count = 3;
}

ParticleSceneNode::~ParticleSceneNode()
{
}

void ParticleSceneNode::draw_this(QOpenGLFunctions *func, glm::mat4 matrix, DrawInfo &info)
{
    glm::vec3 right, up;
    if (std::shared_ptr<Camera> camera = info.active_camera.lock()) {
        right = camera->right_vector();
        up = camera->up_vector();
    }

    m_program->bind();

    int camera_up_id = m_program->uniformLocation("camera_up");
    int camera_right_id = m_program->uniformLocation("camera_right");
    int mvpID = m_program->uniformLocation("mvp");
    int textureID = m_program->uniformLocation("particle_tex");

    func->glUniformMatrix4fv(mvpID, 1, GL_FALSE, &matrix[0][0]);
    func->glUniform3fv(camera_right_id, 1, &right[0]);
    func->glUniform3fv(camera_up_id, 1, &up[0]);
    //func->glUniform1i(textureID, 0);


    //draw particle buffers
    m_vao.bind();

    int vertsID = m_program->attributeLocation("loc");
    int billboardID = m_program->attributeLocation("billboard_vert");
    int uvID = m_program->attributeLocation("uv");

    //vertices loc
    m_particle_verts_buffer.bind();
    m_program->enableAttributeArray(vertsID);
    m_program->setAttributeBuffer(vertsID, GL_FLOAT, m_loc_offset, m_loc_count, m_loc_stride);
    m_particle_verts_buffer.release();

    //billboard
    m_square_buffer.bind();
    m_program->enableAttributeArray(billboardID);
    m_program->setAttributeBuffer(billboardID, GL_FLOAT, 0, 3, 0);
    m_square_buffer.release();

    //uv
    m_tex_uv_buffer.bind();
    m_program->enableAttributeArray(uvID);
    m_program->setAttributeBuffer(uvID, GL_FLOAT, 0, 2, 0);
    m_tex_uv_buffer.release();

    glVertexAttribDivisor(vertsID, 1);
    glVertexAttribDivisor(billboardID, 0);
    glVertexAttribDivisor(uvID, 0);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, m_billboard_verts_count, m_particle_verts_count);

    m_program->disableAttributeArray(vertsID);

    m_vao.release();
    m_program->release();
}

void ParticleSceneNode::update_this(UpdateContainer updates)
{
    qDebug() << "update particles";

    //update particle system state
    m_particle_system->update(updates);

    //fetch particle system state and feed buffers
    ParticleGroup pGroup = m_particle_system->getParticles();

    if (std::shared_ptr<ParticleState> particles = pGroup.particles.lock()) {
        m_particle_verts_buffer.bind();
        m_particle_verts_buffer.allocate(particles->data(), particles->size() * sizeof(Particle));
        m_particle_verts_buffer.release();

        m_loc_stride = pGroup.stride;
        m_loc_offset = pGroup.position_offset;
        m_loc_count  = pGroup.position_count;

        m_particle_verts_count = particles->size();

        qDebug() << m_loc_offset << m_loc_count << m_loc_stride;
    }
}
