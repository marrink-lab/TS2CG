#if !defined(AFX_IndexFileReader_H_334B21B8_C13C_5648_BF23_124095086234__INCLUDED_)
#define AFX_IndexFileReader_H_334B21B8_C13C_5648_BF23_124095086234__INCLUDED_


#include "Def.h"
#include "Vec3D.h"
#include "UnitCell.h"
#include "IndexGroup.h"


class IndexFileReader
{
public:
    
    IndexFileReader(std::string filename);
	 ~IndexFileReader();


        inline  std::vector<IndexGroup *> GetGroupIndex()                const  {return m_pGroupIndex;}




public:
    
  void GenerateGroupsFromIndex();    // a function for changing the x position of a IndexFileReader



public:


private:
    std::vector<IndexGroup *>  m_pGroupIndex;
    std::vector<IndexGroup >  m_GroupIndex;


    std::string m_IndexFileName;

};


#endif
