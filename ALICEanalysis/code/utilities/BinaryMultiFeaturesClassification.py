from sklearn.ensemble import GradientBoostingClassifier, RandomForestClassifier, AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from utilitiesGeneral import filterdataframe_pt,splitdataframe_sigbkg,checkdir,preparestringforuproot
import pandas as pd
import numpy as np
import uproot
from sklearn.utils import shuffle


def getvariablestraining(case):
  mylistvariables=[]
  if (case=="Ds"):
    mylistvariables=['d_len_xy_ML','norm_dl_xy_ML','cos_p_ML','cos_p_xy_ML','imp_par_xy_ML','sig_vert_ML',"delta_mass_KK_ML",'cos_PiDs_ML',"cos_PiKPhi_3_ML"]
  if (case=="Lc"):
    mylistvariables=['d_len_ML','d_len_xy_ML','norm_dl_xy_ML','dist_12_ML','cos_p_ML','pt_p_ML','pt_K_ML','pt_pi_ML','sig_vert_ML','dca_ML']
  if ((case=="PIDPion") | (case=="PIDKaon")):
    mylistvariables=['dedx0_ML','tof0_ML','dca0_ML','sigdca0_ML','chisq0_ML','itscl0_ML','tpccl0_ML']
  return mylistvariables

def getvariablesBoundaries(case):
  mylistvariablesboundaries=[]
  if (case=="Ds"):
    mylistvariablesboundaries=['d_len_xy_ML','delta_mass_KK_ML']
  if (case=="Lc"):
    mylistvariablesboundaries=['d_len_xy_ML','dca_ML']
  if ((case=="PIDPion") | (case=="PIDKaon")):
    mylistvariablesboundaries=['dedx0_ML','pdau0_ML']
  return mylistvariablesboundaries


def getvariablesothers(case):
  mylistvariablesothers=[]
  if (case=="Ds" or case=="Lc"):
    mylistvariablesothers=['inv_mass_ML','pt_cand_ML']
  if ((case=="PIDPion") | (case=="PIDKaon")):
    mylistvariablesothers=['pdau0_ML','pdg0_ML']
  return mylistvariablesothers

def getvariableissignal(case):
  myvariablesy=0
  if (case=="Ds" or case=="Lc"):
    myvariablesy='signal_ML'
  if ((case=="PIDPion") | (case=="PIDKaon")):
    myvariablesy='signal_ML'
  return myvariablesy

def getvariablesall(case):
  mylistvariablesall=[]
  if (case=="Ds"):
    mylistvariablesall=['d_len_xy_ML','norm_dl_xy_ML','cos_p_ML','cos_p_xy_ML','imp_par_xy_ML','sig_vert_ML',"delta_mass_KK_ML",'cos_PiDs_ML',"cos_PiKPhi_3_ML",'inv_mass_ML','pt_cand_ML','signal_ML',"cand_type_ML"]
  if (case=="Lc"):
    mylistvariablesall=['inv_mass_ML','pt_cand_ML','d_len_ML','d_len_xy_ML','norm_dl_xy_ML','dist_12_ML','cos_p_ML','pt_p_ML','pt_K_ML','pt_pi_ML','sig_vert_ML','dca_ML','cand_type_ML']
  if ((case=="PIDPion") | (case=="PIDKaon")):
    mylistvariablesall=['dedx0_ML','tof0_ML','dca0_ML','sigdca0_ML','chisq0_ML','itscl0_ML','tpccl0_ML','pdau0_ML','pdg0_ML']
  return mylistvariablesall

def getvariablecorrelation(case):
  mylistvariablesx=[]
  mylistvariablesy=[]
  if (case=="Ds"):
    mylistvariablesx = ['pt_cand_ML','d_len_xy_ML','sig_vert_ML',"pt_cand_ML","pt_cand_ML","norm_dl_xy_ML","cos_PiDs_ML","cos_p_xy_ML","cos_p_xy_ML"]
    mylistvariablesy = ['d_len_xy_ML','sig_vert_ML','delta_mass_KK_ML',"delta_mass_KK_ML","sig_vert_ML","d_len_xy_ML","cos_PiKPhi_3_ML","sig_vert_ML","pt_cand_ML"]
  if (case=="Lc"):
    mylistvariablesx = ['pt_cand_ML','d_len_xy_ML']
    mylistvariablesy = ['d_len_xy_ML','sig_vert_ML']
  if ((case=="PIDPion") | (case=="PIDKaon")):
    mylistvariablesx = ['pdau0_ML','pdau0_ML','itscl0_ML']
    mylistvariablesy = ['dedx0_ML','tof0_ML','chisq0_ML']
  return mylistvariablesx,mylistvariablesy

def getgridsearchparameters(case):
  if (case=="Ds" or case=="Lc"):
    namesCV=["Random_Forest","GradientBoostingClassifier"]
    classifiersCV=[RandomForestClassifier(),GradientBoostingClassifier()]
    param_gridCV = [[{'n_estimators': [3, 10, 50, 100], 'max_features': [2,4,6,8],'max_depth': [1,4]}],[{'learning_rate': [0.01,0.05, 0.1], 'n_estimators': [1000, 2000, 5000],'max_depth' : [1, 2, 4]}]]
    changeparameter=["n_estimators","n_estimators"]
  return namesCV,classifiersCV,param_gridCV,changeparameter
  

def getDataMCfiles(case):
  fileData=""
  fileMC=""
  if (case=="Ds"):
    fileData="/Users/gianmicheleinnocenti/MLproductions/AnalysisResults_Ds_Data_2018Sep21_LHC15o_pass1_pidfix_CandBased_skimmed.root"
    fileMC="/Users/gianmicheleinnocenti/MLproductions/AnalysisResults_Ds_MC_2018Sep21_LHC18a4a2_cent_fast_CandBased_skimmed.root"
  if (case=="Lc"):
    fileData="/Users/gianmicheleinnocenti/MLproductions/AnalysisResults_Lambdac_Data_CandBased_skimmed.root"
    fileMC="/Users/gianmicheleinnocenti/MLproductions/AnalysisResults_Lambdac_MC_CandBased_skimmed.root"
  if ((case=="PIDPion") | (case=="PIDKaon")):
    fileData="/Users/gianmicheleinnocenti/MLproductions/AnalysisResults_TreeForPIDwithML_Dplus_CandBased_skimmed.root"
    fileMC="/Users/gianmicheleinnocenti/MLproductions/AnalysisResults_TreeForPIDwithML_Dplus_CandBased_skimmed.root"
  return fileData,fileMC

def getTreeName(case):
  treename=""
  if (case=="Ds"):
    treename="fTreeDsFlagged"
  if (case=="Lc"):
    treename="fTreeLcFlagged"
  if ((case=="PIDPion") | (case=="PIDKaon")):
    treename="fTreePIDFlagged"
  return treename

def getdataframe(filename,treename,variables):
  file = uproot.open(filename)
  tree = file[treename]
  dataframe=tree.pandas.df(preparestringforuproot(variables))
  return dataframe


def getmasscut(case):
  fmassmin=-1
  fmassmax=-1  
  if (case=="Ds"):
    fmassmin=1.85
    fmassmax=2.04
    
  if (case=="Lc"):
    fmassmin=1.80
    fmassmax=2.04  
  return fmassmin,fmassmax

def getPDGcode(case):
  if (case=="PIDPion"):
    PDGcode=211
    
  if (case=="PIDKaon"):
    PDGcode=321
  return PDGcode


def prepareMLsample(classtype,case,dataframe_data,dataframe_MC,nevents):
  dataframe_ML_joined = pd.DataFrame()
  if(classtype=="HFmeson"):
  
    dataframe_bkg=dataframe_data
    dataframe_sig=dataframe_MC
    fmassmin,fmassmax=getmasscut(case)
    dataframe_sig=dataframe_sig.loc[(dataframe_sig["cand_type_ML"] == 2) | (dataframe_sig["cand_type_ML"] == 3)]
    dataframe_sig['signal_ML'] = 1
    print (dataframe_sig)
    dataframe_bkg=dataframe_bkg.loc[(dataframe_bkg["inv_mass_ML"] < fmassmin) | (dataframe_bkg["inv_mass_ML"] > fmassmax)]
    dataframe_bkg['signal_ML'] = 0
    
  if(classtype=="PID"):
    dataframe_MC["pdg0_ML"]=dataframe_MC["pdg0_ML"].abs()
    dataframe_sig=dataframe_MC.loc[(dataframe_MC["pdg0_ML"] == getPDGcode(case))]
    dataframe_sig['signal_ML'] = 1
    dataframe_bkg=dataframe_MC.loc[(dataframe_MC["pdg0_ML"] != getPDGcode(case))]
    dataframe_bkg['signal_ML'] = 0


  dataframe_sig=dataframe_sig[:nevents]
  dataframe_bkg=dataframe_bkg[:nevents]
  dataframe_ML_joined = pd.concat([dataframe_sig, dataframe_bkg])
  if ((nevents>len(dataframe_sig)) or (nevents>len(dataframe_bkg))):
    print ("------------------------- ERROR: there are not so many events!!!!!! ------------------------- ")
      
  return dataframe_ML_joined



  
