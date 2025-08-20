#ifndef CURVING_H
#define CURVING_H

#include "ReadVoxelModel.h"

void embossing();
void engraving();
void open_work_of_embossing();
void open_work_of_engraving();

class carving_cls
{
public:
    bool tm = false;
    bool emboss = false;
    bool engrave = false;

private:

};

extern carving_cls carving;

extern int maxIndex; // the most outter surface layer
extern int minIndex; // the most inner surface layer

#endif