#if !defined(AFX_VertexMove_H_8P4B21B8_C13C_5648_BF23_914095086215__INCLUDED_)
#define AFX_VertexMove_H_8P4B21B8_C13C_5648_BF23_914095086215__INCLUDED_


#include "SimDef.h"
#include "vertex.h"
#include "triangle.h"
#include "links.h"
#include "Vec3D.h"

class VertexMove
{
public:
    

    VertexMove(vertex *pvertex, double dx, double dy, double dz,Vec3D *Box);
	 ~VertexMove();







public:
    void   Move();
    void   RejectMove();
//  void AddtoVertexList(vertex * z);

private:


private:
double m_dx;
double m_dy;
double m_dz;
double m_X;
double m_Y;
double m_Z;
    double m_oldX;
    double m_oldY;
    double m_oldZ;

vertex *m_pvertex;
vertex  m_vertex;
std::vector <vertex> m_AVer;
std::vector<vertex *> m_pAVer;
std::vector <links> m_RingLinks;
std::vector <links> m_nLinks;
std::vector <triangle> m_Triangle;
double m_oldEnergy;
Vec3D *m_pBox;


std::vector <links> m_LIntEChange;         /// links that may change their interaction energy after a vertex move
std::vector <links*> m_pLIntEChange;      /// pointer to links that may change their interaction energy after a vertex move

//  int m_ID;



    





};


#endif
