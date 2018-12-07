###############################################################
##                                                           ##
##     Software for single-label classification with Scikit  ##
##      Origin: G.M. Innocenti (CERN)(ginnocen@cern.ch)       ##
##                                                           ##
###############################################################

"""
Methods to do hyper-parameters optimization
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import GridSearchCV
from sklearn_evaluation import plot
from utilitiesGeneral import makestring, make_lst_str_2, make_lst_str_set, makecombinations, splitlist


def do_gridsearch(namesCV_,classifiersCV_,mylistvariables_,param_gridCV_,X_train_,y_train_,cv_,ncores):
  grid_search_models_=[]
  grid_search_bests_=[]
  list_scores=[]    # new part for info storing
  for nameCV, clfCV, gridCV in zip(namesCV_, classifiersCV_,param_gridCV_):
    print("--- Looking at ",nameCV)
    grid_search = GridSearchCV(clfCV, gridCV, cv=cv_,scoring='neg_mean_squared_error',n_jobs=ncores)
    grid_search_model=grid_search.fit(X_train_, y_train_)
    cvres = grid_search.cv_results_
    for mean_score, params in zip(cvres["mean_test_score"], cvres["params"]):
      print(np.sqrt(-mean_score), params)
    list_scores.append(pd.DataFrame(cvres)) # new part for info storing
    grid_search_best=grid_search.best_estimator_.fit(X_train_, y_train_)
    print("\n--- Best parameters for %s:\n%s"%(nameCV,grid_search.best_params_))
    grid_search_models_.append(grid_search_model)
    grid_search_bests_.append(grid_search_best)
  return grid_search_models_,grid_search_bests_,list_scores
    

def plot_gridsearch(namesCV_,change_,grid_search_models_,output_,suffix_):
  '''
  NB: function not working for SciKit > 0.17
  '''
  for nameCV,change,gridCV in zip(namesCV_,change_,grid_search_models_):
    figure = plt.figure(figsize=(10,10))
    plot.grid_search(gridCV.grid_scores_, change=change,kind='bar')
    plt.title('Grid search results '+ nameCV, fontsize=17)
    plt.ylim(-0.8,0)
    plt.ylabel('negative mean squared error',fontsize=17)
    plt.xlabel(change,fontsize=17)
    plotname=output_+"/GridSearchResults"+nameCV+suffix_+".png"
    plt.savefig(plotname)

def perform_plot_gridsearch(names,scores,keys,changeparameter,output_,suffix_):
  '''
  Function for grid scores plotting
  '''
  fig = plt.figure(figsize=(35,15))
  for name,score_obj,key,change in zip(names,scores,keys,changeparameter):
    if len(key)==1:
      print("### ADD AT LEAST 1 PARAMETER ###")
      continue
    # get list of unique values for each variable 
    how_many_pars=len(key)
    par_set=[]
    for i_key in key:
      if(i_key!=change):
        par_set.append(set(list(score_obj[i_key])))
    listcomb = []
    if len(par_set)>1:
      listcomb = splitlist(makecombinations(par_set))
    if len(par_set)==1:
      listcomb = makecombinations(par_set)
    # plotting a graph for every combination of paramater different from change (e.g.: n_estimator in random_forest): score vs. change
    pad = plt.subplot(1,len(names),names.index(name)+1)
    pad.set_title(name,fontsize=20)
    plt.ylim(-0.3,0)
    plt.xlabel(change,fontsize=15)
    plt.ylabel('neg_mean_squared_error',fontsize=15)
    pad.get_xaxis().set_tick_params(labelsize=15)
    pad.get_yaxis().set_tick_params(labelsize=15)
    key.remove(change) 
    for case in listcomb:
      df_case = score_obj.copy()
      lab = ""
      for i_case,i_key in zip(case,key):
        df_case = df_case.loc[df_case[i_key]==float(i_case)]
        lab = lab+"{0}: {1} \n".format(i_key,i_case)   
      df_case.plot(x=change,y='mean_test_score',ax=pad,label=lab,marker='o')
    pad.legend(fontsize=10)
  plotname=output_+"/GridSearchResults"+name+suffix_+".png"
  #plt.show()
  plt.savefig(plotname)
