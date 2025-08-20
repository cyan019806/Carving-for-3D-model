#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <stack>
#include <glm/glm.hpp>

struct MatrixStack{
public:
    MatrixStack();

    void Push();
    void Pop();
    bool isEmpty();
    glm::mat4 Top();
    void Save(const glm::mat4 &matrix);
    void Load();
private:
    std::stack<glm::mat4> stack;
};

#endif