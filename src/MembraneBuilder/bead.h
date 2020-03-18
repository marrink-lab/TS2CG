#if !defined(AFX_bead_H_334B21B8_C13C_5648_BF23_124095086234__INCLUDED_)
#define AFX_bead_H_334B21B8_C13C_5648_BF23_124095086234__INCLUDED_


#include "Def.h"
#include "Vec3D.h"
#include "UnitCell.h"

class molecules;
class bead
{
public:
    
	bead(int id, std::string name, std::string type, std::string resname, int resid, double x, double y, double z);
	bead(int id, std::string name, std::string type, std::string resname, int resid);
	 ~bead();


        inline const int GetID()                const  {return m_ID;}
        inline double GetXPos()                        {return m_X;}
        inline double GetYPos()                        {return m_Y;}
        inline double GetZPos()                        {return m_Z;}
        inline std::string GetBeadType()        const        {return m_BeadType;}
        inline std::string GetResName()         const        {return m_ResName;}
        inline int GetResid()         const        {return m_Resid;}
        inline std::string GetBeadName()        const        {return m_BeadName;}
        inline UnitCell *GetBeadUnitCell()        const        {return m_BeadUnitCell;}
        inline molecules *GetBeadMol()        const        {return m_pBeadMol;}
        inline bool Hasmol()        const        {return m_hasMol;}



public:
    
  void UpdateXPos(double x);    // a function for changing the x position of a bead
  void UpdateYPos(double y);
  void UpdateZPos(double z);
  void UpdateBox(Vec3D* z);
  void UpdatePos(Vec3D* B, double x, double y, double z);
  void UpdatePos(double x, double y, double z);
  void UpdateBeadUnitCell(UnitCell * z);
  void UpdateBeadMol(molecules * z);
    void UpdateHasMol(bool z);

public:


private:

    bool m_hasMol;
  double m_X;
  double m_Y;
  double m_Z;
  std::string m_BeadName;
  std::string m_BeadType;
  std::string m_ResName;
  int m_Resid;
  int m_ID;
  Vec3D *m_pBox;
  UnitCell *m_BeadUnitCell;
  molecules *m_pBeadMol;

};


#endif
