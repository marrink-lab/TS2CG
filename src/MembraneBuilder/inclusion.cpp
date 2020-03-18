

#include <stdio.h>
#include "inclusion.h"
inclusion::inclusion(int id, int typeID, int pointid, Vec3D D )
{


    m_Direction = D;
    m_ID = id;
    m_TypeID = typeID;
    m_PointID = pointid;
}


inclusion::~inclusion()
{
    
}
















