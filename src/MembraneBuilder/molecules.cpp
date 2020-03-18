

#include <stdio.h>
#include "molecules.h"
molecules::molecules(int id, std::string name, std::vector <bead*> mpbeads)
{

m_ID = id;
m_MolName = name;
    m_pAllBeads = mpbeads;

}


molecules::~molecules()
{
    
}


void molecules::CalculateMolCM()
{
    
    
    for (std::vector<bead* >::iterator it = m_pAllBeads.begin() ; it != m_pAllBeads.end(); ++it)
    {
        
        
    }

}
