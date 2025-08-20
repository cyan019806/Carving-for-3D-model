#include "matrixStack.h"
#include <stack>
#include <iostream>

MatrixStack::MatrixStack(){
    stack.push(glm::mat4(1.0f));
}

void MatrixStack::Push() {
    stack.push(stack.top());
}

void MatrixStack::Pop() {
    if(isEmpty()) {
        std::cerr << "The matrix stack is empty!!!" << std::endl;
        return;
    }
    stack.pop();
}

glm::mat4 MatrixStack::Top() {
    return stack.top();
}

void MatrixStack::Save(const glm::mat4 &matrix) {
    stack.pop();
    stack.push(matrix);
}

void MatrixStack::Load() {
    stack.pop();
    stack.push(glm::mat4(1.0f));
}

bool MatrixStack::isEmpty() {
    return stack.empty();
}