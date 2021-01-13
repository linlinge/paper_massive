#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "PCLExtend.h"
#include "FileExtend.h"
#include <random>
#include <algorithm>
#include<stdlib.h>
#include<time.h> 
using namespace std;
struct Position
{
    float x,y,z;    
};
struct PositionNormal{
    float x,y,z;
    float nx,ny,nz;
};
struct PositionNormalColorA{
    float x,y,z;
    float nx,ny,nz;
    unsigned char r,g,b,alpha;
};
struct PositionColorNormal{
    float x,y,z;
    unsigned char r,g,b;
    int nx,ny,nz;
};
struct PositionColorScalar{
    float x,y,z;
    unsigned char r,g,b;
    float scalar;
};
struct PositionColorAlpha{
    float x,y,z;
    unsigned char r,g,b,alpha;
};

enum MODE{POSITION,POSITION_NORMAL,POSITION_NORMAL_COLOR,POSITION_NORMAL_COLORA,POSITION_COLOR_NORMAL,POSITION_COLOR_SCALAR,POSITION_COLOR_ALPHA};

class PlyParsing
{
    private:
        FILE* fp_;
        /* header parsing paramters */
        int length_of_header;
        char char_header[1000];
        int mode;
        int vertex_num;

    public:
        void Open(string path);
        void ParseHeader();
        void ReadLine(pcl::PointCloud<PointType>::Ptr cloud, int itmp, string filepath,int linenum);
        void PrintHeader(string filepath);
        void LoadPly(string path);
        void Close();
};


class Pipline
{
    // private:

    public:
       void Downsampling(string filepath);       
};