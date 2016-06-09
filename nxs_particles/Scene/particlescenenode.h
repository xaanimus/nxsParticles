#ifndef PARTICLESCENENODE_H
#define PARTICLESCENENODE_H

#include <QtGui>
#include "scenenode.h"
#include "Particles/particlesystem.h"

class ParticleSceneNode : public SceneNode
{
public:
    ParticleSceneNode();
    ~ParticleSceneNode();

private:
    virtual void draw_this(QOpenGLFunctions *func, glm::mat4 matrix, DrawInfo &info);
    virtual void update_this(UpdateContainer updates);

    QOpenGLBuffer m_particle_verts_buffer,
                  m_square_buffer,
                  m_tex_uv_buffer;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_program;

    GLuint m_texture_id;

    std::shared_ptr<ParticleSystem> m_particle_system;

    int m_particle_verts_count;
    int m_billboard_verts_count;
    int m_tex_uv_count;
    int m_loc_stride, m_loc_offset, m_loc_count;
};

#endif // PARTICLESCENENODE_H
