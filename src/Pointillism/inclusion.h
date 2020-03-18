#if !defined(AFX_inclusion_H_8P4B21B8_C13C_5648_BF23_124095086234__INCLUDED_)
#define AFX_inclusion_H_8P4B21B8_C13C_5648_BF23_124095086234__INCLUDED_


#include "SimDef.h"
#include "CNTCell.h"
#include "Vec3D.h"
class vertex;
class inclusion
{
public:
    
	inclusion(int id);
	 ~inclusion();


	    inline const int GetID()                const  {return m_ID;}
        inline std::vector <double> GetKappa()                        {return m_kappa;}
     	inline std::vector <double> GetC0()                        	  {return m_C0;}
        inline std::string GetType()                                  {return m_Type;}
        inline int GetTypeID()                                  {return m_TypeID;}
        inline vertex* Getvertex()                                    {return m_pvertex;}
        inline int GetNSymmetry()                                    {return m_NSymmetry;}
        inline Vec3D GetLDirection()                                    {return m_LDirection;}
  //      inline Vec3D GetGDirection()                                    {return m_GDirection;}


public:
    

  void UpdateC0(double ,double ,double );
  void UpdateKappa(double ,double ,double ,double);
  void UpdateType(std::string ,int );
  void Updatevertex(vertex * );
  void UpdateNSymmetry(int );
  void UpdateLocalDirection(Vec3D );
//  void UpdateGlobalDirection(Vec3D );

public:

    void ReadInclusionFromFile(std::ifstream *inputfile,std::vector <vertex *> pv);
    
    void WriteInclusionToFile(std::ofstream *output);
    void WriteInclusion();
private:



    std::vector <double> m_kappa;
    std::vector <double> m_C0;
    int m_ID;
    int m_TypeID;
    int m_NSymmetry;
    std::string m_Type;
    Vec3D m_LDirection;      /// its direction in the local frame
    Vec3D m_GDirection;       /// its direction in the global frame
    vertex *m_pvertex;
    









};


#endif
