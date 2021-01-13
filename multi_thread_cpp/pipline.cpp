#include "pipline.h"
void PlyParsing::Open(string path)
{
    const char* fname=path.c_str();
	fp_ = fopen(fname, "rb");
	if (fp_ == NULL)
	{
		cout << "file " << fname << "does not exist" << endl;		
	}
    mode=-1;
}

void PlyParsing::ParseHeader()
{
    length_of_header=0;    
    fread((void*)(&char_header), sizeof(char_header), 1, fp_);
    string str_tmp=char_header;    
    length_of_header=str_tmp.find("end_header")+10;

    /* which type of file? */
    int pos_xyz=str_tmp.find("property float x");
    int pos_rgb=str_tmp.find("property uchar red");
    int pos_nxyz=str_tmp.find("property float nx");
    int pos_scalar=str_tmp.find("property float scalar_Scalar_field");
    int pos_alpha=str_tmp.find("property uchar alpha");
    vertex_num= atoi(str_tmp.substr(str_tmp.find("element vertex")+15, str_tmp.find("property float")-str_tmp.find("element vertex")-15).c_str());

    if((pos_xyz !=-1) && (pos_nxyz==-1) && (pos_rgb==-1) && (pos_scalar==-1)) mode=POSITION;
    else if((pos_xyz+51==pos_rgb) && (pos_rgb + 60 == pos_scalar)) mode=POSITION_COLOR_SCALAR;
    else if((pos_xyz+51==pos_rgb) && (pos_rgb + 60 == pos_alpha)) mode=POSITION_COLOR_ALPHA;
    else if((pos_xyz+51==pos_nxyz) && (pos_rgb==-1) && (pos_scalar==-1))   mode=POSITION_NORMAL;
    else if((pos_xyz + 51 == pos_rgb) && (pos_rgb + 60 == pos_nxyz) && (pos_scalar==-1) ) mode=POSITION_COLOR_NORMAL;
    else if((pos_xyz + 51 == pos_nxyz) && (pos_nxyz+54 == pos_rgb) && (pos_scalar==-1)) mode=POSITION_NORMAL_COLORA;
}

void PlyParsing::PrintHeader(string filepath)
{
    Open(filepath);
    fseek(fp_,0,0);
    fread((void*)(&char_header), sizeof(char_header), 1, fp_);
    string str_tmp=char_header;
    length_of_header=str_tmp.find("end_header")+10;
    str_tmp=str_tmp.substr(0,length_of_header);
    cout<<str_tmp<<endl;
    Close();
}

void PlyParsing::ReadLine(pcl::PointCloud<PointType>::Ptr cloud,int itmp, string filepath, int linenum)
{
    FILE* fp=fopen(filepath.c_str(), "rb");
    if(POSITION==mode){
        Position ptmp;          
        long int start_cursor=length_of_header+sizeof(ptmp)*linenum+1;
        fseek(fp,start_cursor,0);
        fread((void*)(&ptmp), sizeof(ptmp), 1, fp);      

	   
        cloud->points[itmp].x=ptmp.x;
        cloud->points[itmp].y=ptmp.y;
        cloud->points[itmp].z=ptmp.z;        
    }
    else if(POSITION_COLOR_SCALAR==mode){
        PositionColorScalar ptmp;          
        long int start_cursor=length_of_header+sizeof(ptmp)*linenum+1;
        fseek(fp,start_cursor,0);
        fread((void*)(&ptmp), sizeof(ptmp), 1, fp);      
	    
        cloud->points[itmp].x=ptmp.x;
        cloud->points[itmp].y=ptmp.y;
        cloud->points[itmp].z=ptmp.z;
        cloud->points[itmp].r=ptmp.r;
        cloud->points[itmp].g=ptmp.g;
        cloud->points[itmp].b=ptmp.b;        
    }
    else if(POSITION_COLOR_ALPHA==mode){
        PositionColorAlpha ptmp;          
        long int start_cursor=length_of_header+sizeof(ptmp)*linenum+1;
        int flag=fseek(fp,start_cursor,0);
        if(flag!=0) cout<<"error!"<<endl;
        fread((void*)(&ptmp), sizeof(ptmp), 1, fp); 
        cloud->points[itmp].x=ptmp.x;
        cloud->points[itmp].y=ptmp.y;
        cloud->points[itmp].z=ptmp.z;
        cloud->points[itmp].r=ptmp.r;
        cloud->points[itmp].g=ptmp.g;
        cloud->points[itmp].b=ptmp.b;
    }
    else if(POSITION_NORMAL==mode){ // position normal        
        PositionNormal ptmp;          
        long int start_cursor=length_of_header+sizeof(ptmp)*linenum+1;
        fseek(fp,start_cursor,0);
        fread((void*)(&ptmp), sizeof(ptmp), 1, fp);      
        cloud->points[itmp].x=ptmp.x;
        cloud->points[itmp].y=ptmp.y;
        cloud->points[itmp].z=ptmp.z;
        cloud->points[itmp].normal_x=ptmp.nx;
        cloud->points[itmp].normal_y=ptmp.ny;
        cloud->points[itmp].normal_z=ptmp.nz;
    }
    else if(POSITION_COLOR_NORMAL==mode) // Position Color Normal
    {
        PositionColorNormal ptmp;         
        long int start_cursor=length_of_header+sizeof(ptmp)*linenum+1;
        fseek(fp,start_cursor,0);
        fread((void*)(&ptmp), sizeof(ptmp), 1, fp); 
        cloud->points[itmp].x=ptmp.x;
        cloud->points[itmp].y=ptmp.y;
        cloud->points[itmp].z=ptmp.z;
        cloud->points[itmp].r=ptmp.r;
        cloud->points[itmp].g=ptmp.g;
        cloud->points[itmp].b=ptmp.b;
        cloud->points[itmp].normal_x=ptmp.nx;
        cloud->points[itmp].normal_y=ptmp.ny;
        cloud->points[itmp].normal_z=ptmp.nz;
    }
    else if(POSITION_NORMAL_COLORA==mode){
        PositionNormalColorA ptmp;         
        long int start_cursor=length_of_header+sizeof(ptmp)*linenum+1;
        fseek(fp,start_cursor,0);
        fread((void*)(&ptmp), sizeof(ptmp), 1, fp);         
        cloud->points[itmp].x=ptmp.x;
        cloud->points[itmp].y=ptmp.y;
        cloud->points[itmp].z=ptmp.z;
        cloud->points[itmp].r=ptmp.r;
        cloud->points[itmp].g=ptmp.g;
        cloud->points[itmp].b=ptmp.b;
        cloud->points[itmp].normal_x=ptmp.nx;
        cloud->points[itmp].normal_y=ptmp.ny;
        cloud->points[itmp].normal_z=ptmp.nz;        
    }
    fclose(fp);
}

void PlyParsing::LoadPly(string path)
{
	Open(path);
	ParseHeader();
    
    // const int SEED = 666;
    // default_random_engine dre(SEED);
    // uniform_int_distribution<int> d(0, vertex_num-1);
    vector<int> v;
    cout<<"number of vertex:"<<vertex_num<<endl;
    int LEN = (int)vertex_num/100;
    v.resize(LEN);
    #pragma omp parallel for
    for (int i = 0; i < LEN; ++i)        
        v[i]=rand() % vertex_num;

    // WriteVector("/home/llg/dataset_paper/1.txt",v);
    // sort(v.begin(),v.end());
    // vector<int>::iterator it=(v.begin(),v.end());
    // v.erase(it,v.end());
    

    pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
    cloud->points.resize(v.size());

    #pragma omp parallel for
    for(int i=0;i<v.size();i++){
        // cout<<"linenum:"<<v[i]+1<<endl;
        ReadLine(cloud,i,path,v[i]);
        // ReadLine(cloud,i);
    }
    pcl::io::savePLYFileBinary("/home/llg/dataset_paper/out_of_core_version.ply",*cloud);

    /* get down-sampling point cloud with pcl */
    // pcl::PointCloud<PointType>::Ptr cloud_raw(new pcl::PointCloud<PointType>);
    // pcl::io::loadPLYFile(path,*cloud_raw);
    // subtract_points(cloud_raw,v,true);
    // pcl::io::savePLYFileBinary("/home/llg/dataset_paper/raw_version.ply",*cloud_raw);
   
	// int vertex_num = 13469;
	// int face_num = 26934;
	// for (int i = 0; i < vertex_num; i++) {
	// 	float x, y, z;
	// 	fread((void*)(&x), 4, 1, fp_); x=x/1000;
	// 	fread((void*)(&y), 4, 1, fp_); y=y/1000;
	// 	fread((void*)(&z), 4, 1, fp_); z=z/1000;
	// 	float nx, ny, nz;
	// 	fread((void*)(&nx), 4, 1, fp_);
	// 	fread((void*)(&ny), 4, 1, fp_);
	// 	fread((void*)(&nz), 4, 1, fp_);
	// 	unsigned char r, g, b, a;
	// 	fread((void*)(&r), 1, 1, fp_);
	// 	fread((void*)(&g), 1, 1, fp_);
	// 	fread((void*)(&b), 1, 1, fp_);
	// 	fread((void*)(&a), 1, 1, fp_);
	// 	// Vertex vertex;
	// 	// vertex.x = x / 1000;
	// 	// vertex.y = y / 1000;
	// 	// vertex.z = z / 1000;
	// 	// vertex.nx = nx;
	// 	// vertex.ny = ny;
	// 	// vertex.nz = nz;
	// 	// vertex.r = r;
	// 	// vertex.g = g;
	// 	// vertex.b = b;
	// 	// vertex.a = a;
	// 	// vertices.push_back(vertex);
    //     cout<<x<<endl;
	// }
	// for (int i = 0; i < face_num; i++) {
	// 	unsigned char n;
	// 	int v1, v2, v3;
	// 	fread((unsigned char*)(&n), 1, 1, fp_);
	// 	fread((void*)(&v1), 4, 1, fp_);
	// 	fread((void*)(&v2), 4, 1, fp_);
	// 	fread((void*)(&v3), 4, 1, fp_);
	// 	// Face face;
	// 	// face.v1 = v1;
	// 	// face.v2 = v2;
	// 	// face.v3 = v3;
	// 	// faces.push_back(face);
	// }
}

void PlyParsing::Close()
{
    fclose(fp_);
}

void Pipline::Downsampling(string filepath)
{

}