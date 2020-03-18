#if !defined(AFX_CNTCell_H_8P4B21B8_C13C_5648_BF23_444095086234__INCLUDED_)
#define AFX_CNTCell_H_8P4B21B8_C13C_5648_BF23_444095086234__INCLUDED_


#include "SimDef.h"
#include "Vec3D.h"
class vertex;
class CNTCell
{
public:
    

	CNTCell(int id, int i, int j, int k);
	 ~CNTCell();



        inline const std::vector <int>   GetIndex()    const    {return m_Index;}
        inline const int GetID()                const  		{return m_ID;}
        inline std::vector <vertex *> GetVertexList()           {return m_VertexList;} 
	inline Vec3D	GetCNTSidemin()              		{return m_CNTRmin;}
	inline Vec3D	GetCNTSidemax()              		{return m_CNTRmax;}
        inline std::vector <CNTCell *> GetVNeighbourCNTCell()   {return m_VNeighbourCNTCell;}




public:
    

  void AddtoVertexList(vertex * z);
  void UpdateCNTSidemin(Vec3D);
  void UpdateCNTSidemax(Vec3D);
  void AddtoNeighbourCNTCell(CNTCell* z);
  void RemoveFromVertexList(vertex * z);
    CNTCell * ReturnNewCell(int,int,int);

private:



private:


  int m_ID;
  std::vector <vertex *> m_VertexList;
  std::vector <CNTCell *> m_VNeighbourCNTCell;
  std::vector <int>  m_Index;
  Vec3D		m_CNTRmin;
  Vec3D		m_CNTRmax;



    





};


#endif
