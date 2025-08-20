#include <model.h>
#include <iostream>

model_cls voxelmodel;

model_cls::model_cls()
{
}

model_cls::~model_cls()
{
}

bool model_cls::isValidPosition(glm::ivec3 pos)
{
    // check if the position in resolution range or not
    return pos.x >= 0 && pos.x < rawModel.infData.resolution[0] &&
           pos.y >= 0 && pos.y < rawModel.infData.resolution[1] &&
           pos.z >= 0 && pos.z < rawModel.infData.resolution[2];
}

Node *model_cls::findNodeByPos(glm::ivec3 pos)
{
    // find the pointer with position
    auto it = nodeMap.find(pos);
    if (it != nodeMap.end())
    {
        return it->second;
    }
    return nullptr;
}

void model_cls::setNeighbors(Node *head)
{
    Node *current = head;
    while (current != nullptr)
    {
        glm::ivec3 pos = current->pos;
        std::vector<glm::ivec3> neighborOffsets = {
            glm::ivec3(-1, 0, 0), glm::ivec3(1, 0, 0), // left right
            glm::ivec3(0, -1, 0), glm::ivec3(0, 1, 0), // up down
            glm::ivec3(0, 0, -1), glm::ivec3(0, 0, 1)  // front back
        };

        // find neighbors
        int num = 0;
        for (auto &offset : neighborOffsets)
        {
            glm::ivec3 neighborPos = pos + offset;
            if (isValidPosition(neighborPos)) // check if the position is in resolution range
            {
                Node *neighbor = findNodeByPos(neighborPos); // according position to find neighbors
                if (neighbor != nullptr)
                {
                    current->neighbors.push_back(neighbor); // put pointer of neighbor to array 
                    num++;
                }
            }
        }
        current->weight = 1.0 / (double)(num + 1);
        current = current->next;
    }
}

void model_cls::AddInitNode(Node **head, Node **tail, double value, glm::ivec3 pos, int current_state)
{
    Node *newNode = new Node();
    newNode->distance = value;
    newNode->pos = pos;
    newNode->state = current_state;
    newNode->weight = 0.0f;
    newNode->next = nullptr;

    // put node add to nodeMap
    nodeMap[pos] = newNode;

    if (*head == nullptr) // first node
    {
        *head = newNode;
        *tail = newNode;
        return;
    }
    else
    {
        (*tail)->next = newNode;
        *tail = newNode;
        return;
    }
}

void model_cls::AddNode(Node **head, Node **tail, Node *target)
{
    target->next = nullptr; // Clear the next pointer, as it's now in a new list

    if (*tail == nullptr)
    {
        *head = target;
        *tail = target;
        // Renew the new pointer in the neighbors' neighbors
        // nodeMap[target->pos] = target;
        // RenewNeighborOfNeighbor(target);
    }
    else
    {
        (*tail)->next = target;
        *tail = target;
        // Renew the new pointer in the neighbors' neighbors
        // nodeMap[target->pos] = target;
        // RenewNeighborOfNeighbor(target);
    }
}

// Node *model_cls::DeleteFirstNode(Node *head)
// {
//     if (head == NULL)
//         return NULL;
//     Node *tmp = head;
//     head = head->next;
//     CloseList = head;
//     delete tmp;

//     return head;
// }

// Node *model_cls::DeleteNode(Node *current)
// {
//     if (current == NULL || current->next == NULL)
//         return NULL;
//     current->pos.x = current->next->pos.x;
//     current->pos.y = current->next->pos.y;
//     current->pos.z = current->next->pos.z;
//     current->distance = current->next->distance;
//     current->neighbor_num = current->next->neighbor_num;
//     for (int n = 0; n < current->neighbor_num; n++)
//     {
//         current->neighbor[n] = current->next->neighbor[n];
//         current->neighbor_state[n] = current->next->neighbor_state[n];
//     }
//     current->next = current->next->next;

//     return current;
// }

Node *model_cls::GetCLOSE(Node *CLOSE)
{
    Node *head = CLOSE;
    // renew the neighbors of neighbor
    // nodeMap[head->pos] = head;
    // RenewNeighborOfNeighbor(head);
    return head;
}

glm::ivec3 model_cls::SetCoefficient(int last_state, int next_state, double last_dist, double next_dist)
{
    if (last_state == 0 && next_state != 0) // DONE & !=DONE
    {
        return COEF_LAST;
    }
    else if (last_state != 0 && next_state == 0) // !=DONE & DONE
    {
        return COEF_NEXT;
    }
    else if (last_state == 0 && next_state == 0) // both DONE
    {
        if (last_dist <= next_dist)
            return COEF_LAST;
        else
            return COEF_NEXT;
    }
    else
        return COEF_ERROR;
}

glm::dvec3 model_cls::GetCoefficient(glm::ivec3 Coef, double u_last, double u_next)
{
    glm::dvec3 coef;
    coef.x = Coef.y * Coef.y;
    coef.y = 2 * (Coef.x * Coef.y * u_last + Coef.y * Coef.z * u_next);
    coef.z = ((Coef.x * u_last) * (Coef.x * u_last)) + ((Coef.z * u_next) * (Coef.z * u_next));

    return coef;
}

double model_cls::Compute(Node *neighbor)
{
    int num_x = 0, num_y = 0, num_z = 0;
    // find the neighbor number of x, y, z direction
    for (auto neighbor_of_neighbor : neighbor->neighbors)
    {
        if (neighbor_of_neighbor->pos.x != neighbor->pos.x && neighbor_of_neighbor->pos.y == neighbor->pos.y && neighbor_of_neighbor->pos.z == neighbor->pos.z)
            num_x++;
        if (neighbor_of_neighbor->pos.x == neighbor->pos.x && neighbor_of_neighbor->pos.y != neighbor->pos.y && neighbor_of_neighbor->pos.z == neighbor->pos.z)
            num_y++;
        if (neighbor_of_neighbor->pos.x == neighbor->pos.x && neighbor_of_neighbor->pos.y == neighbor->pos.y && neighbor_of_neighbor->pos.z != neighbor->pos.z)
            num_z++;
    }

    glm::ivec3 CoefX, CoefY, CoefZ;
    glm::dvec3 Coefx, Coefy, Coefz;

    if (num_x == 2 && num_y == 2 && num_z == 2)
    {
        // X direction
        CoefX = SetCoefficient(neighbor->neighbors[0]->state, neighbor->neighbors[1]->state, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);

        // Y direction
        CoefY = SetCoefficient(neighbor->neighbors[2]->state, neighbor->neighbors[3]->state, neighbor->neighbors[2]->distance, neighbor->neighbors[3]->distance);
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[2]->distance, neighbor->neighbors[3]->distance);

        // Z direction
        CoefZ = SetCoefficient(neighbor->neighbors[4]->state, neighbor->neighbors[5]->state, neighbor->neighbors[4]->distance, neighbor->neighbors[5]->distance);
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[4]->distance, neighbor->neighbors[5]->distance);
    }
    else if (num_x == 2 && num_y == 2 && num_z == 1)
    {
        // X direction
        CoefX = SetCoefficient(neighbor->neighbors[0]->state, neighbor->neighbors[1]->state, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);

        // Y direction
        CoefY = SetCoefficient(neighbor->neighbors[2]->state, neighbor->neighbors[3]->state, neighbor->neighbors[2]->distance, neighbor->neighbors[3]->distance);
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[2]->distance, neighbor->neighbors[3]->distance);

        // Z direction
        if (neighbor->pos.z > neighbor->neighbors[4]->pos.z) // z - 1
            CoefZ = COEF_LAST;
        else // z + 1
            CoefZ = COEF_NEXT;
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[4]->distance, neighbor->neighbors[4]->distance);
    }
    else if (num_x == 2 && num_y == 1 && num_z == 2)
    {
        // X direction
        CoefX = SetCoefficient(neighbor->neighbors[0]->state, neighbor->neighbors[1]->state, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);

        // Y direction
        if (neighbor->pos.y > neighbor->neighbors[2]->pos.y) // y - 1
            CoefY = COEF_LAST;
        else // y + 1
            CoefY = COEF_NEXT;
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[2]->distance, neighbor->neighbors[2]->distance);

        // Z direction
        CoefZ = SetCoefficient(neighbor->neighbors[3]->state, neighbor->neighbors[4]->state, neighbor->neighbors[3]->distance, neighbor->neighbors[4]->distance);
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[3]->distance, neighbor->neighbors[4]->distance);
    }
    else if (num_x == 1 && num_y == 2 && num_z == 2)
    {
        // X direction
        if (neighbor->pos.x > neighbor->neighbors[0]->pos.x) // x - 1
            CoefX = COEF_LAST;
        else // x + 1
            CoefX = COEF_NEXT;
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[0]->distance);

        // Y direction
        CoefY = SetCoefficient(neighbor->neighbors[1]->state, neighbor->neighbors[2]->state, neighbor->neighbors[1]->distance, neighbor->neighbors[2]->distance);
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[1]->distance, neighbor->neighbors[2]->distance);

        // Z direction
        CoefZ = SetCoefficient(neighbor->neighbors[3]->state, neighbor->neighbors[4]->state, neighbor->neighbors[3]->distance, neighbor->neighbors[4]->distance);
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[3]->distance, neighbor->neighbors[4]->distance);
    }
    else if (num_x == 2 && num_y == 1 && num_z == 1)
    {
        // X direction
        CoefX = SetCoefficient(neighbor->neighbors[0]->state, neighbor->neighbors[1]->state, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[1]->distance);

        // Y direction
        if (neighbor->pos.y > neighbor->neighbors[2]->pos.y) // y - 1
            CoefY = COEF_LAST;
        else // y + 1
            CoefY = COEF_NEXT;
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[2]->distance, neighbor->neighbors[2]->distance);

        // Z direction
        if (neighbor->pos.z > neighbor->neighbors[3]->pos.z) // z - 1
            CoefZ = COEF_LAST;
        else // z + 1
            CoefZ = COEF_NEXT;
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[3]->distance, neighbor->neighbors[3]->distance);
    }
    else if (num_x == 1 && num_y == 2 && num_z == 1)
    {
        // X direction
        if (neighbor->pos.x > neighbor->neighbors[0]->pos.x) // x - 1
            CoefX = COEF_LAST;
        else // x + 1
            CoefX = COEF_NEXT;
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[0]->distance);

        // Y direction
        CoefY = SetCoefficient(neighbor->neighbors[1]->state, neighbor->neighbors[2]->state, neighbor->neighbors[1]->distance, neighbor->neighbors[2]->distance);
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[1]->distance, neighbor->neighbors[2]->distance);

        // Z direction
        if (neighbor->pos.z > neighbor->neighbors[3]->pos.z) // z - 1
            CoefZ = COEF_LAST;
        else // z + 1
            CoefZ = COEF_NEXT;
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[3]->distance, neighbor->neighbors[3]->distance);
    }
    else if (num_x == 1 && num_y == 1 && num_z == 2)
    {
        // X direction
        if (neighbor->pos.x > neighbor->neighbors[0]->pos.x) // x - 1
            CoefX = COEF_LAST;
        else // x + 1
            CoefX = COEF_NEXT;
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[0]->distance);

        // Y direction
        if (neighbor->pos.y > neighbor->neighbors[1]->pos.y) // y - 1
            CoefY = COEF_LAST;
        else // y + 1
            CoefY = COEF_NEXT;
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[1]->distance, neighbor->neighbors[1]->distance);

        // Z direction
        CoefZ = SetCoefficient(neighbor->neighbors[2]->state, neighbor->neighbors[3]->state, neighbor->neighbors[2]->distance, neighbor->neighbors[3]->distance);
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[2]->distance, neighbor->neighbors[3]->distance);
    }
    else if (num_x == 1 && num_y == 1 && num_z == 1)
    {
        // X direction
        if (neighbor->pos.x > neighbor->neighbors[0]->pos.x) // x - 1
            CoefX = COEF_LAST;
        else // x + 1
            CoefX = COEF_NEXT;
        Coefx = GetCoefficient(CoefX, neighbor->neighbors[0]->distance, neighbor->neighbors[0]->distance);

        // Y direction
        if (neighbor->pos.y > neighbor->neighbors[1]->pos.y) // y - 1
            CoefY = COEF_LAST;
        else // y + 1
            CoefY = COEF_NEXT;
        Coefy = GetCoefficient(CoefY, neighbor->neighbors[1]->distance, neighbor->neighbors[1]->distance);

        // Z direction
        if (neighbor->pos.z > neighbor->neighbors[2]->pos.z) // z - 1
            CoefZ = COEF_LAST;
        else // z + 1
            CoefZ = COEF_NEXT;
        Coefz = GetCoefficient(CoefZ, neighbor->neighbors[2]->distance, neighbor->neighbors[2]->distance);
    }

    double a, b, c, u_now;
    a = Coefx.x + Coefy.x + Coefz.x;
    b = Coefx.y + Coefy.y + Coefz.y;
    c = Coefx.z + Coefy.z + Coefz.z - 1.0;

    u_now = ((-1 * b) + sqrt((b * b) - (4 * a * c))) / (2 * a);
    return u_now;
}

Node *model_cls::DeleteTargetFromClose(Node *target, Node *head)
{
    // Case when target is at the head of the list
    if (head == target)
    {
        Node *newHead = head->next;
        head = newHead; // Update the head
        // // renew the neighbors of neighbor
        // nodeMap[target->pos] = target;
        // RenewNeighborOfNeighbor(target);
        return head;
    }

    // Traverse the list to find the target node
    Node *previous = head;
    Node *current = head->next;

    while (current != nullptr)
    {
        if (current == target) // Target node found
        {
            previous->next = current->next; // Unlink the node
            // renew the neighbors of neighbor
            // nodeMap[target->pos] = target;
            // RenewNeighborOfNeighbor(target);
            return head; // Return the head of the list
        }
        previous = current;
        current = current->next;
    }
    // renew the neighbors of neighbor
    // nodeMap[target->pos] = target;
    // RenewNeighborOfNeighbor(target);
    return head;
}

Node *model_cls::RemoveFromCloseList(Node *node)
{
    // Case when the node to remove is the head of the list
    if (CloseHead == node)
    {
        CloseHead = CloseHead->next; // Update the head to the next node
        // Renew the new pointer in the neighbors' neighbors
        // nodeMap[node->pos] = node;
        // RenewNeighborOfNeighbor(node);
        return CloseHead; // Return the new head
    }

    // Traverse the list to find the node to remove
    Node *current = CloseHead;
    while (current != nullptr && current->next != nullptr)
    {
        if (current->next == node)
        {
            current->next = node->next; // Unlink the node
            // Renew the new pointer in the neighbors' neighbors
            // nodeMap[node->pos] = node;
            // RenewNeighborOfNeighbor(node);
            return CloseHead; // Return the head of the list
        }
        current = current->next; // Move to the next node
    }

    return CloseHead; // Return the head of the list if nothing was removed
}

Node *model_cls::InsertIntoCloseList(Node *node, Node *head)
{
    // if CLOSE is null, insert in head
    if (head == nullptr || node->distance < head->distance)
    {
        node->next = head;
        head = node;

        return head;
    }
    Node *current = head;
    while (current->next != nullptr && current->next->distance < node->distance)
    {
        current = current->next;
    }
    node->next = current->next;
    current->next = node;

    return head;
}

Node *model_cls::InsertCloseList(Node *neighbor)
{
    // If the Close list is empty or neighbor needs to be inserted at the head
    if (CloseHead == nullptr || neighbor->distance <= CloseHead->distance)
    {
        neighbor->next = CloseHead; // Insert at the head
        CloseHead = neighbor;
        // Renew the new pointer in the neighbors' neighbors
        // nodeMap[neighbor->pos] = neighbor;
        // RenewNeighborOfNeighbor(neighbor);
        return CloseHead;
    }

    Node *previous = CloseHead;
    Node *current = CloseHead->next;

    // Traverse the list to find the correct position
    while (current != nullptr && neighbor->distance > current->distance)
    {
        previous = current;
        current = current->next;
    }

    // Insert the neighbor between previous and current
    previous->next = neighbor;
    neighbor->next = current;

    // Renew the new pointer in the neighbors' neighbors
    // nodeMap[neighbor->pos] = neighbor;
    // RenewNeighborOfNeighbor(neighbor);
    return CloseHead;
}

void model_cls::RenewNeighborOfNeighbor(Node *node)
{
    for (auto &neighbor : node->neighbors)
    {
        for (auto &neighbor_of_neighbor : neighbor->neighbors)
        {
            if (neighbor_of_neighbor->pos == node->pos)
            {
                // Update the reference in the actual neighbor's neighbors list
                neighbor_of_neighbor = nodeMap[node->pos];
            }
        }
    }
}

// Node* model_cls::UpdateCloseList(Node *node, Node *head) {
//     // if CLOSE list is null or node need to insert to head
//     if (head == nullptr || node->distance <= head->distance) {
//         // if node is not head node, remove it
//         if (node != head) {
//             Node *previous = nullptr;
//             Node *current = head;
//             // find the precious node
//             while (current != nullptr && current != node) {
//                 previous = current;
//                 current = current->next;
//             }
//             // add node between previous->next and node->next
//             if (current == node && previous != nullptr) {
//                 previous->next = node->next;
//             }

//             // add node to head
//             node->next = head;
//             head = node;
//             // only when node move need to renew nodeMap and neighbors 
//             nodeMap[node->pos] = node;
//             RenewNeighborOfNeighbor(node);
//         }
//         return head;
//     }

//     Node *previous = nullptr;
//     Node *current = head;
//     // trival list and find remove node and new position
//     while (current != nullptr) {
//         // find the correct position to insert
//         if (node->distance < current->distance) {
//             // if node at correct position, then do not change
//             if (previous == node || current == node) {
//                 return head;
//             }
//             break;
//         }
//         previous = current;
//         current = current->next;
//     }
//     // sure node not point to itself, avoid to lost node
//     if (current != node) {
//         // insert node
//         node->next = current;
//         if (previous != nullptr) {
//             previous->next = node;

//             // only when node move need to renew nodeMap and neighbors
//             nodeMap[node->pos] = node;
//             RenewNeighborOfNeighbor(node);
//         }
//     }

//     return head;
// }

void model_cls::ComputeCLOSE(Node *neighbor)
{
    double value = Compute(neighbor);

    if (value < neighbor->distance) // new distance smaller than old -> rewnew
    {
        neighbor->distance = value;
        rawModel.voxelData[neighbor->pos.y][neighbor->pos.x][neighbor->pos.z] = value;
        // lock CLOSE list to avoid change at the same time
        {
            std::lock_guard<std::mutex> lock(closeListMutex);
            // CloseHead = UpdateCloseList(neighbor, CloseHead);
            CloseHead = RemoveFromCloseList(neighbor);  // remove from CLOSE list
            CloseHead = InsertCloseList(neighbor);      // insert node to CLOSE list according distance
            // bacause the position of memory is change need to renew neighbor
            if (neighbor->state != 1) { // only if state change need to renew neighbors
                nodeMap[neighbor->pos] = neighbor;
                RenewNeighborOfNeighbor(neighbor);
            }
        }
    }
}

void model_cls::RemoveFromFarList(Node *node)
{
    if (FarHead == node)
    {
        FarHead = FarHead->next;
        return;
    }
    Node *current = FarHead;
    while (current->next != nullptr)
    {
        if (current->next == node)
        {
            current->next = node->next;
            return;
        }
        current = current->next;
    }
}

void model_cls::ComputeFAR(Node *neighbor)
{
    double value = Compute(neighbor);

    if (value < neighbor->distance) // new distance smaller than old -> need to renew
    {
        neighbor->distance = value;
        rawModel.voxelData[neighbor->pos.y][neighbor->pos.x][neighbor->pos.z] = value;
        neighbor->state = 1;
        // lock FAR list to avoid change at the same time
        {
            std::lock_guard<std::mutex> lock(closeListMutex);
            CloseHead = InsertIntoCloseList(neighbor, CloseHead); // add to CLOSE
            // bacause the position of memory is change need to renew neighbor
            nodeMap[neighbor->pos] = neighbor;
            RenewNeighborOfNeighbor(neighbor);
        }
        {
            std::lock_guard<std::mutex> lock(farListMutex);
            RemoveFromFarList(neighbor); // remove from FAR
            // Renew the new pointer in the neighbors' neighbors
            // nodeMap[neighbor->pos] = neighbor;
            // RenewNeighborOfNeighbor(neighbor);
        }
    }
    else // new distance bigger than old
    {
        neighbor->state = 0; // state -> 0 and add to DONE
        // // renew neighbor
        // nodeMap[neighbor->pos] = neighbor;
        // RenewNeighborOfNeighbor(neighbor);
        {
            std::lock_guard<std::mutex> lock(doneListMutex);
            AddNode(&DoneHead, &DoneTail, neighbor);  // add to DONE
            // bacause the position of memory is change need to renew neighbor
            nodeMap[neighbor->pos] = neighbor;
            RenewNeighborOfNeighbor(neighbor);
        }
        {
            std::lock_guard<std::mutex> lock(farListMutex);
            RemoveFromFarList(neighbor); // remove from FAR
            // Renew the new pointer in the neighbors' neighbors
            // nodeMap[neighbor->pos] = neighbor;
            // RenewNeighborOfNeighbor(neighbor);
        }
    }
}
// rendering
Node *model_cls::StartFastMarching(Node *head)
{
    // Every time add the Minimum distance to DONE
    Node *target = GetCLOSE(head);
    // std::cout << "target: (" << target->pos.x << ", " << target->pos.y << ", " << target->pos.z << " ) " << "state: " << target->state << " dist: " << target->distance << std::endl;
    target->state = 0;
    // Remove target from CLOSE list first, so we don't lose the next pointer
    CloseHead = DeleteTargetFromClose(target, head);
    // bacause the position of memory is change need to renew neighbor
    // nodeMap[target->pos] = target;
    // RenewNeighborOfNeighbor(target);
    // cout
    // std::cout << "CloseList: " << std::endl;
    // Node* tmp = CloseHead;
    // while (tmp != nullptr)
    // {
    //     std::cout << "( " << tmp->pos.x << ", " << tmp->pos.y << ", " << tmp->pos.z << " ) ";
    //     tmp = tmp->next;
    // }
    // std::cout << std::endl;
    target->next = nullptr;

    // move the target node to DONE
    AddNode(&DoneHead, &DoneTail, target);
    // renew neighbor
    nodeMap[target->pos] = target;
    RenewNeighborOfNeighbor(target);
    // cout
    // Node *tmp = DoneHead;
    // std::cout << "DoneList: " << std::endl;
    // while (tmp != nullptr)
    // {
    //     std::cout << "( " << tmp->pos.x << ", " << tmp->pos.y << ", " << tmp->pos.z << " )" << " ";
    //     tmp = tmp->next;
    // }
    // std::cout << std::endl;

    for (auto neighbor : target->neighbors)
    {
        if (neighbor->state == 1) // CLOSE
        {
            ComputeCLOSE(neighbor);
        }
        else if (neighbor->state == 2) // FAR
        {
            ComputeFAR(neighbor);
        }
    }
    // CloseHead = DeleteTargetFromClose(target);
    // cout
    // std::cout << "CloseList: " << std::endl;
    // tmp = CloseHead;
    // while (tmp != nullptr)
    // {
    //     std::cout << "( " << tmp->pos.x << ", " << tmp->pos.y << ", " << tmp->pos.z << " ) ";
    //     tmp = tmp->next;
    // }
    // std::cout << std::endl;

    return CloseHead;
}

void model_cls::CreateLinkedList()
{
    DoneHead = nullptr, CloseHead = nullptr, FarHead = nullptr;
    DoneTail = nullptr, CloseTail = nullptr, FarTail = nullptr;

    for (int i = 0; i < rawModel.infData.resolution[2]; i++)
    {
        for (int j = 0; j < rawModel.infData.resolution[1]; j++)
        {
            for (int k = 0; k < rawModel.infData.resolution[0]; k++)
            {
                // input all node to DONE, CLOSE, FAR, according their voxelData
                if (rawModel.voxelData[i][j][k] == 0) // DONE
                {
                    AddInitNode(&DoneHead, &DoneTail, 0, glm::ivec3(j, i, k), 0);
                }
                else if (rawModel.voxelData[i][j][k] == 1) // CLOSE
                {
                    AddInitNode(&CloseHead, &CloseTail, 1, glm::ivec3(j, i, k), 1);
                }
                else // FAR
                {
                    AddInitNode(&FarHead, &FarTail, MAX_DIST, glm::ivec3(j, i, k), 2);
                }
            }
        }
    }
    // set neighbors
    setNeighbors(DoneHead);
    setNeighbors(CloseHead);
    setNeighbors(FarHead);

    std::cout << "create linked list end!" << std::endl;
    // Node* tmp = DoneHead;
    // while(tmp != nullptr)
    // {
    //     std::cout << "Node: (" << tmp->pos.x << ", " << tmp->pos.y << ", " << tmp->pos.z << ") " << "state: " << tmp->state << " dist: " << tmp->distance << std::endl;
    //     for(auto neighbor : tmp->neighbors)
    //     {
    //         std::cout << "( " << neighbor->pos.x << ", " << neighbor->pos.y << ", " << neighbor->pos.z << " ) " << "state: " << neighbor->state << " dist: " << neighbor->distance << std::endl;
    //     }
    //     tmp = tmp->next;
    // }
}

void model_cls::ScanLine()
{
    // 1: x -- y
    int first = 0, encounterZeroTimes = 0;
    bool flag = false;
    for (int y = 0; y < rawModel.infData.resolution[2]; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1]; x++) // +x direction from y = min scan to y = max - 1
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int z = 0; z < rawModel.infData.resolution[0]; z++)
            {
                if (rawModel.voxelData[y][x][z] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                    {
                        // rawModel.voxelData[y][x][z] *= -1;
                        rawModel.voteData[y][x][z]++;
                    }
                    else
                    {
                        if (rawModel.voxelData[y][x][z] == 0.0f)
                        {
                            if (z - 1 > 0)
                            {
                                if (rawModel.voxelData[y][x][z - 1] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                // rawModel.voxelData[i][j][k] *= -1;
                                rawModel.voteData[y][x][z]++;
                            }
                        }
                    }
                }
            }
        }
    }
    // 2: -x -- y
    first = 0, encounterZeroTimes = 0;
    flag = false;
    for (int y = rawModel.infData.resolution[2] - 1; y >= 0; y--)
    {
        for (int x = rawModel.infData.resolution[1] - 1; x >= 0; x--) // -x direction from y = max - 1 scan to y = min
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int z = rawModel.infData.resolution[0] - 1; z >= 0; z--)
            {
                if (rawModel.voxelData[y][x][z] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                    {
                        if (rawModel.voxelData[y][x][z] > 0.0f)
                        {
                            // rawModel.voxelData[y][x][z] *= -1;
                            rawModel.voteData[y][x][z]++;
                        }
                    }
                    else
                    {
                        if (rawModel.voxelData[y][x][z] == 0.0f)
                        {
                            if (z + 1 < rawModel.infData.resolution[0])
                            {
                                if (rawModel.voxelData[y][x][z + 1] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                // rawModel.voxelData[i][j][k] *= -1;
                                rawModel.voteData[y][x][z]++;
                            }
                        }
                    }
                }
            }
        }
    }
    // 3: +x -- z
    first = 0, encounterZeroTimes = 0;
    flag = false;
    for (int z = 0; z < rawModel.infData.resolution[0]; z++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1]; x++) // +x direction from z = min scan to z = max - 1
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int y = 0; y < rawModel.infData.resolution[2]; y++)
            {
                if (rawModel.voxelData[y][x][z] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                    {
                        // rawModel.voxelData[y][x][z] *= -1;
                        rawModel.voteData[y][x][z]++;
                    }
                    else
                    {
                        if (rawModel.voxelData[y][x][z] == 0.0f)
                        {
                            if (y - 1 > 0)
                            {
                                if (rawModel.voxelData[y - 1][x][z] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                // rawModel.voxelData[i][j][k] *= -1;
                                rawModel.voteData[y][x][z]++;
                            }
                        }
                    }
                }
            }
        }
    }
    // 4: -x -- z 
    first = 0, encounterZeroTimes = 0;
    flag = false;
    for (int z = rawModel.infData.resolution[0] - 1; z >= 0; z--)
    {
        for (int x = rawModel.infData.resolution[1] - 1; x >= 0; x--) // -x direction from z = max - 1 scan to z = min
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int y = rawModel.infData.resolution[2] - 1; y >= 0; y--)
            {
                if (rawModel.voxelData[y][x][z] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                    {
                        if (rawModel.voxelData[y][x][z] > 0.0f)
                        {
                            // rawModel.voxelData[y][x][z] *= -1;
                            rawModel.voteData[y][x][z]++;
                        }
                    }
                    else
                    {
                        if (rawModel.voxelData[y][x][z] == 0.0f)
                        {
                            if (y + 1 < rawModel.infData.resolution[2])
                            {
                                if (rawModel.voxelData[y + 1][x][z] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                // rawModel.voxelData[i][j][k] *= -1;
                                rawModel.voteData[y][x][z]++;
                            }
                        }
                    }
                }
            }
        }
    }
    // 5: +y -- z
    first = 0, encounterZeroTimes = 0;
    flag = false;
    for (int z = 0; z < rawModel.infData.resolution[0]; z++)
    {
        for (int y = 0; y < rawModel.infData.resolution[2]; y++) // +y direction from z = min scan to z = max - 1
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int x = 0; x < rawModel.infData.resolution[1]; x++)
            {
                if (rawModel.voxelData[y][x][z] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                    {
                        // rawModel.voxelData[y][x][z] *= -1;
                        rawModel.voteData[y][x][z]++;
                    }
                    else
                    {
                        if (rawModel.voxelData[y][x][z] == 0.0f)
                        {
                            if (x - 1 > 0)
                            {
                                if (rawModel.voxelData[y][x - 1][z] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                // rawModel.voxelData[i][j][k] *= -1;
                                rawModel.voteData[y][x][z]++;
                            }
                        }
                    }
                }
            }
        }
    }
    // 6: -y -- z
    first = 0, encounterZeroTimes = 0;
    flag = false;
    for (int z = rawModel.infData.resolution[0] - 1; z >= 0; z--)
    {
        for (int y = rawModel.infData.resolution[2] - 1; y >= 0; y--) // -y direction from z = max - 1 scan to z = min
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int x = rawModel.infData.resolution[1] - 1; x >= 0; x--)
            {
                if (rawModel.voxelData[y][x][z] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                    {
                        if (rawModel.voxelData[y][x][z] > 0.0f)
                        {
                            // rawModel.voxelData[y][x][z] *= -1;
                            rawModel.voteData[y][x][z]++;
                        }
                    }
                    else
                    {
                        if (rawModel.voxelData[y][x][z] == 0.0f)
                        {
                            if (x + 1 < rawModel.infData.resolution[1])
                            {
                                if (rawModel.voxelData[y][x + 1][z] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                // rawModel.voxelData[i][j][k] *= -1;
                                rawModel.voteData[y][x][z]++;
                            }
                        }
                    }
                }
            }
        }
    }
    // according the six direction to decide inner or outer voxel
    for (int y = 0; y < rawModel.infData.resolution[2]; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1]; x++)
        {
            for (int z = 0; z < rawModel.infData.resolution[0]; z++)
            {
                if(rawModel.voteData[y][x][z] >= 3)
                    rawModel.voxelData[y][x][z] *= -1;
            }
        }
    }
}

void model_cls::FindMinAndMaxDist()
{
    for (int i = 0; i < rawModel.infData.resolution[2]; i++)
    {
        for (int j = 0; j < rawModel.infData.resolution[1]; j++)
        {
            for (int k = 0; k < rawModel.infData.resolution[0]; k++)
            {
                if (rawModel.voxelData[i][j][k] < rawModel.minDist)
                {
                    rawModel.minDist = rawModel.voxelData[i][j][k];
                }
                if (rawModel.voxelData[i][j][k] > rawModel.maxDist)
                {
                    rawModel.maxDist = rawModel.voxelData[i][j][k];
                }
            }
        }
    }
    std::cout << "minDist: " << rawModel.minDist << ", maxDist: " << rawModel.maxDist << std::endl;
}

double model_cls::LaplacianSmoothing(Node* target)
{
    int count = 0;
    double sum = 0.0;
    double w_neighbor = 0.0;

    float x = target->pos.x;
    float y = target->pos.y;
    float z = target->pos.z;

	// if target is model or boundary keep original distance
	if(target->distance == 0 || (x == rawModel.infData.resolution[1] - 1) || (y == rawModel.infData.resolution[2] - 1) || (z == rawModel.infData.resolution[0] - 1))
	{
        return target->distance;
	}

    // compute sum of distance of neighbors
    for (auto neighbor : target->neighbors)
    {
        double value = neighbor->distance;
        sum += value;
        w_neighbor += neighbor->weight;
        count++;
    }

    if(count == 0)
    {
        return target->distance;
    }
    else
    {
        double w_target = target->weight;
        // double w_neighbor = (1 - target->weight);
        double new_value = (target->weight * target->distance + w_neighbor * (sum / count));
        return new_value;
    }
}

void model_cls::sortedInsert(Node *newnode)
{
    // special case for the head end
    if (sorted == NULL || sorted->distance > newnode->distance)
    {
        newnode->next = sorted;
        sorted = newnode;
    }
    else
    {
        Node *current = sorted;
        // locate the node before the point of insertion
        while (current->next != NULL && current->next->distance <= newnode->distance)
        {
            current = current->next;
        }
        newnode->next = current->next;
        current->next = newnode;
    }
}

Node *model_cls::InsectionSort(Node **head, Node *current, Node *precious)
{
    Node *start = *head;
    Node *start_next = start->next;
    Node *current_next = current->next;
    while (start_next != NULL)
    {
        if (current->distance >= start->distance && current->distance < start_next->distance) // find the position to insert and this node is not insert node
        {
            if (current->pos.x == start->pos.x && current->pos.y == start->pos.y && current->pos.z == start->pos.z)
                break;
            Node *tmp = start_next;
            Node *tmp2 = current_next;
            start->next = current;
            current->next = tmp;
            precious->next = tmp2;
            break;
        }
        start = start->next;
        start_next = start->next;
    }
    return *head;
}

void model_cls::FreeNode(Node *node)
{
    free(node);
}

/*
void model_cls::ScanLine()
{
    int first = 0, encounterZeroTimes = 0;
    bool flag = false;
    for (int i = 0; i < rawModel.infData.resolution[2]; i++)
    {
        for (int j = 0; j < rawModel.infData.resolution[1]; j++) // +x direction from y = min scan to y = max - 1
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int k = 0; k < rawModel.infData.resolution[0]; k++)
            {
                if (rawModel.voxelData[i][j][k] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                        rawModel.voxelData[i][j][k] *= -1;
                    else
                    {
                        if (rawModel.voxelData[i][j][k] == 0.0f)
                        {
                            if (k - 1 > 0)
                            {
                                if (rawModel.voxelData[i][j][k - 1] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                rawModel.voxelData[i][j][k] *= -1;
                                // rawModel.voteData[i][j][k]++;
                            }
                        }
                    }
                }
            }
        }
    }
    first = 0, encounterZeroTimes = 0;
    flag = false;
    for (int i = rawModel.infData.resolution[2] - 1; i >= 0; i--)
    {
        for (int j = rawModel.infData.resolution[1] - 1; j >= 0; j--) // -x direction from y = max - 1 scan to y = min
        {
            first = 0;
            encounterZeroTimes = 0;
            flag = false;
            for (int k = rawModel.infData.resolution[0] - 1; k >= 0; k--)
            {
                if (rawModel.voxelData[i][j][k] == 0.0f)
                {
                    if (first == 0) // first time scan to model surface
                    {
                        first++;
                        encounterZeroTimes++;
                    }
                }
                else
                {
                    if (first == 0) // before scan to model surface, mark to outside point, distance * -1
                    {
                        if (rawModel.voxelData[i][j][k] > 0.0f)
                            rawModel.voxelData[i][j][k] *= -1;
                    }
                    else
                    {
                        if (rawModel.voxelData[i][j][k] == 0.0f)
                        {
                            if (k + 1 < rawModel.infData.resolution[0])
                            {
                                if (rawModel.voxelData[i][j][k + 1] != 0)
                                {
                                    encounterZeroTimes++;
                                }
                            }
                        }
                        else
                        {
                            flag = false;
                            if (encounterZeroTimes % 2 == 0)
                            {
                                rawModel.voxelData[i][j][k] *= -1;
                            }
                        }
                    }
                }
            }
        }
    }
    // for (int i = 0; i < rawModel.infData.resolution[2]; i++)
    // {
    //     for (int j = 0; j < rawModel.infData.resolution[1]; j++)
    //     {
    //         for (int k = 0; k < rawModel.infData.resolution[0]; k++)
    //         {
    //             if(i == 8)
    //                 std::cout << rawModel.voxelData[i][j][k] << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << std::endl;
    // }
}
*/