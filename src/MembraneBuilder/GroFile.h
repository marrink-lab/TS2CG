#if !defined(AFX_GroFile_H_334B2143_C13C_5648_BF23_124095086234__INCLUDED_)
#define AFX_GroFile_H_334B2143_C13C_5648_BF23_124095086234__INCLUDED_


#include "Def.h"
#include "Vec3D.h"
#include "bead.h"


class GroFile
{
public:
    
    GroFile(std::string gmxfilename);
	 ~GroFile();


        inline std::vector<bead*> GetpAllBeads()         {return m_pAllBeads;}
        inline std::vector<bead> GetAllBeads()         {return m_AllBeads;}
        inline std::string GetTitle()         {return m_Title;}

        inline Vec3D *GetBox()         {return m_pBox;}



public:
    



private:


    std::vector<bead> m_AllBeads;
    std::vector<bead*> m_pAllBeads;
    std::string m_GroFileName;
    Vec3D m_Box;
    Vec3D *m_pBox;
    std::string m_Title;


private:
    void ReadGroFile(std::string file);
    void WriteGroFile(std::string file);
    void AddBead(bead b);
};


#endif
