#include <string>
#include "TFile.h"
void removeTree(TString file_name, TString object_to_remove = "tree_Lc2V0bachelor;*"){

  TFile *file=new TFile(file_name.Data(),"update");
  file->cd("PWGHF_TreeCreator");
  gDirectory->Delete(object_to_remove.Data());
  file->Close();
}
