/* This code was developed by Weria Pezeshkian at Univeristy of Groningen
 
 Copyright (c) Weria Pezeshkian
 */
#include "Def.h"
#include "Job.h"




int main(int argc, char* argv[])
{


    
 
/// This part get the name of the input file as command line
//// for later use we set all of them in a vector
    std::vector <std::string> argument;
    std::string Temprory;
    if(argc == 1)     // Note that argv[0] = executable file name hence argc >= 1
	{
		// This means, the user want to run the command without any argument         


            std::cout << "Error: no enough argument to perform the analysis";
           // std::cout<<"\n"<<"For more information and tips execute NPCross -h"<<"\n";
            return 0;
        
  
    }
    else
    {
           for (long i=0;i<argc;i++)
           {
               Temprory.assign(argv[i]);
               argument.push_back(Temprory);
           }
  
Job job(argument);

   }

    return 0;

}
