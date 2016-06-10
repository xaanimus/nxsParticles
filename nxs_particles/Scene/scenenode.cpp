#include "scenenode.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"

SceneNode::SceneNode()
    : m_scale(glm::vec3(1.0,1.0,1.0))
{}

void SceneNode::draw(glm::mat4 matrix, DrawInfo &info)
{
    matrix = glm::scale(matrix, m_scale);
    matrix = glm::translate(matrix, m_pos);
    matrix = matrix * glm::orientate4(m_rot);

    draw_this(matrix, info);

    for(SceneNode &child : m_children) {
        child.draw(matrix, info);
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

void GroupSceneNode::draw_this(glm::mat4, DrawInfo &info)
{}
void GroupSceneNode::update_this(UpdateContainer)
{}
