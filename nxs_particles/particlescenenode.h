#ifndef PARTICLESCENENODE_H
#define PARTICLESCENENODE_H

#include <QtGui>
#include "scenenode.h"
#include "particlesystem.h"

class ParticleSceneNode : public SceneNode
{
public:
    ParticleSceneNode();
    ~ParticleSceneNode();

private:
    virtual void draw_this(QOpenGLFunctions *func, glm::mat4 matrix);
    virtual void update_this(UpdateContainer updates);


    QOpenGLBuffer m_particle_verts_buffer;
    QOpenGLBuffer m_square_buffer;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_program;

    std::unique_ptr<ParticleSystem> m_particle_system;

    int m_particle_verts_count;
};

#endif // PARTICLESCENENODE_H
