#if !defined(AFX_WriteFiles_H_7F4B21B8_D13C_9321_BF23_124095086234__INCLUDED_)
#define AFX_WriteFiles_H_7F4B21B8_D13C_9321_BF23_124095086234__INCLUDED_

#include "SimDef.h"
#include "triangle.h"
#include "vertex.h"
#include "links.h"
#include "Vec3D.h"

class WriteFiles
{
public:
    
	WriteFiles(Vec3D *Box );
	 ~WriteFiles();




public:

void Writevtu(std::vector<vertex* > ver, std::vector<triangle* > triangle,  std::vector<links* > , std::string Filename);

void WritevtuNochange(std::vector<vertex* > ver, std::vector<triangle* > triangle,  std::vector<links* > , std::string Filename);
void Writefullvtu(std::vector<vertex* > ver, std::vector<triangle* > triangle,  std::vector<links* > , std::string Filename);
void WriteInclusion(std::string id, std::vector<vertex* > ver, std::ofstream *Output);
    void    Writevtunew(std::vector< vertex* > ver, std::vector< triangle* > triangle1,  std::string Filename);
private:


Vec3D *m_pBox;

  



    





};


#endif
