###############################################################
##                                                           ##
##     Software for single-label classification with Scikit  ##
##      Origin: G.M. Innocenti (CERN)(ginnocen@cern.ch)       ##
##                                                           ##
###############################################################

"""
Methods to: load and write data to ROOT files
            filter and manipulate pandas DataFrames
"""

from ROOT import TFile
from utilitiesRoot import FillNTuple
import sys, os
import uproot
import itertools

def filterdataframe_pt(dataframe_,pt_var_,ptmin_,ptmax_):
  dataframe_ptsel_=dataframe_.loc[(dataframe_[pt_var_] > ptmin_ ) & (dataframe_[pt_var_] < ptmax_ )]
  return dataframe_ptsel_
  
def splitdataframe_sigbkg(dataframe_,var_signal_):
  dataframe_sig_=dataframe_.loc[dataframe_[var_signal_] == 1]
  dataframe_bkg_=dataframe_.loc[dataframe_[var_signal_] == 0]
  return dataframe_sig_,dataframe_bkg_

def checkdir(path):
  if not os.path.exists(path):
    os.makedirs(path)
    
def progressbar(part,tot):
  sys.stdout.flush()      
  length=100
  perc = part/tot
  num_dashes = int(length*perc)
  print("\r[",end='')
  for i in range(0,num_dashes+1):
    print("#",end='')
  for i in range(0,length-num_dashes-1):
    print("-",end='')
  print("] {0:.0%}".format(perc),end='')

def preparestringforuproot(myarray):
  arrayfinal=[]
  for str in myarray:
    arrayfinal.append(str+"*")
  return arrayfinal

def getdataframe(filename,treename,variables):
  file = uproot.open(filename)
  tree = file[treename]
  dataframe=tree.pandas.df(preparestringforuproot(variables))
  return dataframe
  
def getdataframeDataMC(filenameData,filenameMC,treename,variables):
  dfData = getdataframe(filenameData,treename,variables)
  dfMC = getdataframe(filenameMC,treename,variables)
  return dfData,dfMC

def filterdataframe(dataframe_,var_list,minlist_,maxlist_):
  dataframe_sel=dataframe_
  for var, min, max in zip(var_list, minlist_,maxlist_):
    dataframe_sel=dataframe_sel.loc[(dataframe_sel[var] > min ) & (dataframe_sel[var] < max)]
  return dataframe_sel

def filterdataframeDataMC(dfData,dfMC,var_skimming,varmin,varmax):
  dfData_sel = filterdataframe(dfData,var_skimming,varmin,varmax)
  dfMC_sel = filterdataframe(dfMC,var_skimming,varmin,varmax)
  return dfData_sel,dfMC_sel
  
def createstringselection(var_skimming_,minlist_,maxlist_):
  string_selection="dfselection_"
  for var, min, max in zip(var_skimming_, minlist_,maxlist_):
   string_selection=string_selection+(("%s_%.1f_%.1f") % (var,min,max))
  return string_selection

def writeTree(filename,treename,dataframe):
  listvar=list(dataframe)
  values=dataframe.values
  fout = TFile.Open(filename,"recreate")
  FillNTuple(treename,values,listvar)



# stuff for grid search plotting (mfaggin)

def makestring(tpl):
  lst = list(tpl)
  string = ""
  for i in range(0,len(lst)):
    string+=str(lst[i])
    if(i<len(lst)-1):
      string+="/"
  return string

def make_lst_str_set(sets):
  comb = list(itertools.product(*sets))
  # list of strings
  lst_str = []
  for tpl in comb:
    lst_str.append(makestring(tpl))
  return lst_str

def make_lst_str_2(sets,ix,lst_str):
  i=ix
  lst_str_2=[]
  comb=list(itertools.product(lst_str,sets[i]))
  for tpl in comb:
    lst_str_2.append(makestring(tpl))
  i+=1
  if(i==len(sets)):
    return lst_str_2
  else:
    return make_lst_str_2(sets,i,lst_str_2)

def makecombinations(sets):
  if len(sets)>1:
    lst_str = make_lst_str_set(sets)
    ix=2
    if ix==len(sets):
      return lst_str
    else:
      lstout = make_lst_str_2(sets,ix,lst_str)
      return lstout
  if len(sets)==1:
    lstout_unit = []
    for inum in sets[0]:
      lst_tmp = [str(inum)]
      lstout_unit.append(lst_tmp)
    return lstout_unit

def splitlist(lst):
  listout=[]
  for entry in lst:
    listout.append(entry.split("/"))
  return listout

