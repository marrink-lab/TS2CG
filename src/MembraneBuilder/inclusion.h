#if !defined(AFX_inclusion_H_999B21B8_C13C_5648_BF23_124095086444__INCLUDED_)
#define AFX_inclusion_H_999B21B8_C13C_5648_BF23_124095086444__INCLUDED_


#include "Def.h"
#include "Vec3D.h"

class inclusion
{
public:
    
	inclusion(int id, int typeID, int pointid,Vec3D D );
    ~inclusion();


        inline const int GetID()                const  {return m_ID;}
    inline const int GetTypeID()                const  {return m_TypeID;}

    inline const int GetPointID()                const  {return m_PointID;}

        inline Vec3D  GetDirection()                        {return m_Direction;}



public:
    



public:


private:


    Vec3D m_Direction;
    int m_ID;
    int m_TypeID;
    int m_PointID;


};


#endif
