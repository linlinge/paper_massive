#include <iostream>	
#include "PCLExtend.h"
#include <Eigen/Dense>
#include "V3.hpp"
#include <vector>
#include "pipline.h"
using namespace std;

bool is_binary(string path)
{
	int c;
	std::ifstream a(path);
	while((c = a.get()) != EOF && c <= 127);
	a.close();
	if(c == EOF) {
		/* file is all ASCII */
		return false;
	}
	else 
		return true;
	
}

int main(int argc,char** argv)
{
	// ifstream fin("/home/llg/dataset_paper/4_Mimosa_binary.ply", ios::in);
	// string line;
	// for(int i=0;i<20;i++){
	// 	getline(fin,line);
	// 	cout<<line<<endl;
	// }
	// fin.close();

	string filepath="/home/llg/dataset_upholstery/0_raw/luming.ply";
	PlyParsing pps;	
	pps.PrintHeader(filepath);
	pps.LoadPly(filepath);

	// if(is_binary(filepath)==true){
	// 	cout<<"binary"<<endl;
	// 	ifstream fin(filepath, ios::in | ios::binary);
	// 	string line;
	// 	for(int i=0;i<20;i++){
	// 		getline(fin,line);
	// 		cout<<line<<endl;
	// 	}
	// 	fin.close();
	// }
	// else{
	// 	ifstream fin(filepath);
	// 	string line;
	// 	for(int i=0;i<20;i++){
	// 		getline(fin,line);
	// 		cout<<line<<endl;
	// 	}
	// 	fin.close();
	// }

	return 0;
}