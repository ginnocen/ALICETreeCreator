#include <iostream>
#include <string>
#include "TFile.h"

using namespace std;

void removeTree(TString file_name, TString object_to_remove = "tree_Lc2V0bachelor;*"){

  TFile *file=new TFile(file_name.Data(),"update");
  file->cd("PWGHF_TreeCreator");
  gDirectory->Delete(object_to_remove.Data());
  file->Close();
}

int main(int argc, char *argv[])
{
  if((argc != 3))
  {
    std::cout << "Wrong number of inputs" << std::endl;
    return 1;
  }
  
  if(argc == 3)
  removeTree(argv[1],argv[2]);
  return 0;
}
