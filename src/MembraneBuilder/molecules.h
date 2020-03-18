#if !defined(AFX_molecules_H_444Q21B8_C13C_5648_BF23_124095086234__INCLUDED_)
#define AFX_molecules_H_444Q21B8_C13C_5648_BF23_124095086234__INCLUDED_


#include "Def.h"
#include "Vec3D.h"
#include "UnitCell.h"
#include "bead.h"


class molecules
{
public:
    
	molecules(int id, std::string name, std::vector <bead*>);
	 ~molecules();


        inline const int GetID()                const  {return m_ID;}
        inline std::string GetMolName()        const        {return m_MolName;}
        inline  std::vector <bead*> GetMolBeads()        const        {return m_pAllBeads;}
        inline  Vec3D  GetMolCM()        const        {return m_MOLCM;}
        inline  double  GetMinR()        const        {return m_minR;}
    


public:
    
    void CalculateMolCM();



private:


private:

  std::vector <bead*> m_pAllBeads;
  std::string m_MolName;
  int m_ID;
  Vec3D m_MOLCM;
  double m_minR;


};


#endif
