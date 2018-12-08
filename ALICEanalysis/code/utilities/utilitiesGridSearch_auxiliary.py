###############################################################
##                                                           ##
##     Software for single-label classification with Scikit  ##
##      Origin: G.M. Innocenti (CERN)(ginnocen@cern.ch)       ##
##                                                           ##
###############################################################

"""
Methods to do hyper-parameters optimization \n
Auxiliary methods for plotting
"""

import itertools

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