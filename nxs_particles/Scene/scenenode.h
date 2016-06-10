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

    void draw(glm::mat4 matrix, DrawInfo &info);
    void update(UpdateContainer updates);
    void add_child(SceneNode &child);

private:
    virtual void draw_this(glm::mat4 matrix, DrawInfo &info) = 0;
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
    virtual void draw_this(glm::mat4 matrix, DrawInfo &info);
    virtual void update_this(UpdateContainer updates);
};

#endif // SCENENODE_H
