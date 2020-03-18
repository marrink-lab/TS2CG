#if !defined(AFX_VMDOutput_H_7F4B21B8_D13C_9321_BF23_124095086234__INCLUDED_)
#define AFX_VMDOutput_H_7F4B21B8_D13C_9321_BF23_124095086234__INCLUDED_

#include "SimDef.h"
#include "vertex.h"
#include "links.h"
#include "Vec3D.h"

class VMDOutput
{
public:
    
	VMDOutput(Vec3D Box, std::vector<vertex* > verm , std::vector<links* > ,std::string Filename);
	 ~VMDOutput();




public:

void WriteGro();
void WriteGro2();
void WriteXTC();

private:

Vec3D m_Box;
std::vector<vertex* > m_pVers;
std::vector<links* > m_pLinks; 
private:
std::string m_Filename;
  



    





};


#endif
