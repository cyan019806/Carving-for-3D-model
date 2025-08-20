#ifndef MODEL_H
#define MODEL_H

#include <time.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <unordered_map>

#include "background.h"
#include "ReadVoxelModel.h"
#include "Object.h"

#define MAX_DIST 1.0e10

typedef struct node
{
    glm::ivec3 pos;
    double distance;
    int state;                     // DONE: 0, CLOSE: 1, FAR: 2
    std::vector<node *> neighbors; // neighbor's pointer
    double weight;                  // weight to smooth the distance
    struct node *next;
} Node;

// define a function to put in position and find the pointer
struct hashFunc
{
    std::size_t operator()(const glm::ivec3 &pos) const
    {
        return std::hash<int>()(pos.x) ^ std::hash<int>()(pos.y) << 1 ^ std::hash<int>()(pos.z) << 2;
    }
};

class model_cls
{
public:
    model_cls();
    ~model_cls();

    // model type
    Node *DoneHead;
    Node *CloseHead;
    Node *FarHead;
    Node *DoneTail;
    Node *CloseTail;
    Node *FarTail;

    int tmp = 0;

    std::unordered_map<glm::ivec3, Node *, hashFunc> nodeMap;
    std::mutex closeListMutex; // use to protect CLOSE list
    std::mutex doneListMutex;  // use to protect DONE list
    std::mutex farListMutex;  // use to protect FAR list

    Node *sorted;

    void CreateLinkedList();
    bool isValidPosition(glm::ivec3 position);
    Node *findNodeByPos(glm::ivec3 pos);
    void setNeighbors(Node *head);
    Node *StartFastMarching(Node *CloseHead);
    void ComputeCLOSE(Node *neighbor);
    void ComputeFAR(Node *neighbor);
    void FreeNode(Node *node);
    void ComputeDistanceFields();
    double LaplacianSmoothing(Node* target);
    void ScanLine();
    void FindMinAndMaxDist();
    // Node *DeleteFirstNode(Node *head);

private:
    // function
    Node *InsectionSort(Node **head, Node *current, Node *precious);
    void sortedInsert(Node *newnode);
    void AddInitNode(Node **head, Node **tail, double value, glm::ivec3 pos, int current_state);
    void AddNode(Node **head, Node **tail, Node *target);
    Node *RemoveFromCloseList(Node *node);
    Node *InsertIntoCloseList(Node *node, Node *head);
    Node* UpdateCloseList(Node *node, Node *head);
    Node *InsertCloseList(Node *neighbor);
    void RemoveFromFarList(Node *node);
    Node *DeleteTargetFromClose(Node *target, Node *head);
    // Node *DeleteNode(Node *current);
    Node *GetCLOSE(Node *CloseList);
    // void GetInitNeighborState(Node *Type);
    double Compute(Node *neighbor);
    void RenewNeighborOfNeighbor(Node *node);
    glm::ivec3 SetCoefficient(int last, int next, double last_dist, double next_dist);
    glm::dvec3 GetCoefficient(glm::ivec3 Coef, double u_last, double u_next);

    glm::ivec3 COEF_LAST = {-1.0, 1.0, 0.0};
    glm::ivec3 COEF_NEXT = {0.0, -1.0, 1.0};
    glm::ivec3 COEF_ERROR = {0.0, 0.0, 0.0};
};

extern model_cls voxelmodel;

#endif