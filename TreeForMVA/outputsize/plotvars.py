import pandas as pd
import pickle as pkl
import numpy as np
import seaborn 
import matplotlib.pyplot as plt

# script to draw variables with pp/PbPb cuts drawn on top
# choose decay type, and ensure path is defined correctly

decaytype = 'LctopK0s'

if decaytype == 'LctopKpi':
    dataframe_bkg = pd.read_pickle('/data/HeavyFlavour/PbPb_data_3050/14-03-2019/377_20190313-2007/picklefiles/merged/1/AnalysisResultsLctopKpiMergedReco.pkl')
    dataframe_sig = pd.read_pickle('/data/HeavyFlavour/pPb_MC/14-03-2019/378_20190313-2116/picklefiles/merged/1/AnalysisResultsLctopKpiMergedReco.pkl')
    list_variables = ['d_len', 'cos_p', 'imp_par_prong0', 'imp_par_prong1', 'imp_par_prong2', 'dca', 'dist_12', 'dist_23', 'sig_vert', 'pt_prong0', 'pt_prong1', 'pt_prong2', 'inv_mass', 'pt_cand']
    #sum d0^2 = 0, max pt tracks = 0.8
    # variables in order defined above
    cuts_PbPb_0_8 = [0.00625, 0.90, 0, 0, 0,  0.0375, 0.025, 0.025, 0.035, 0.5, 0.5, 0.5,2.2864-0.13,4,0,0]
    cuts_PbPb_8_999 = [0.00625, 0.25, 0, 0, 0,  0.0375, 0.0125, 0.0125, 0.045, 0.5, 0.5, 0.5,2.2864-0.13,8,0,0]
    cuts_pp = [0., 0., 0., 0., 0.,   0.05, 0., 0., 0.06, 0.3, 0.3, 0.3,2.2864-0.18,4,0,0]
elif decaytype == 'LctopK0s':
    dataframe_bkg = pd.read_pickle('/data/HeavyFlavour/PbPb_data_3050/14-03-2019/377_20190313-2007/picklefiles/merged/1/AnalysisResultsLctopK0sMergedReco.pkl')
    dataframe_sig = pd.read_pickle('/data/HeavyFlavour/pPb_MC/14-03-2019/379_20190313-2217/pickleLctopK0s/merged/1/AnalysisResultsLctopK0sMergedReco.pkl')
#    list_variables = ['cos_t_star', 'signd0', 'dca_K0s', 'imp_par_K0s', 'd_len_K0s', 'armenteros_K0s', 'ctau_K0s', 'cos_p_K0s', 'pt_prong0', 'pt_prong1', 'pt_prong2', 'imp_par_prong0', 'imp_par_prong1', 'imp_par_prong2', 'inv_mass', 'pt_cand', 'inv_mass_K0s', 'pt_K0s']
    # list varialbles with each entry corresponding to the entry in the cut object (blank if not included in the ttree)
    list_variables = ['inv_mass','', 'inv_mass_K0s','', 'pt_prong0', 'pt_prong1', 'pt_prong2', '', '', 'cos_p_K0s', 'imp_par_prong0', 'imp_par_prong1', '', '', '', 'pt_K0s', '', '', 'signd0', '', '']
    # Lc -> pK0s variables in the order they are defined in the cut object
    cuts_PbPb_0_8= [2.2864-0.2,0.,0.497648-0.05,0.05,0.5,0.0,0.0,1000.,1000.,0.998,3.,1000.,0.,0.,0.,0.5,9999.,-9999.,-9999.,-9999.,1] #Pb-Pb 010 filtering  
    cuts_PbPb_8_999= [2.2864-0.2,0.,0.497648-0.05,0.05,0.5,0.0,0.0,1000.,1000.,0.998,3.,1000.,0.,0.,0.,0.5,9999.,-9999.,-9999.,-9999.,1] #Pb-Pb 010 filtering  
    cuts_pp = [2.2864-0.2,0.,0.497648-0.05,0.05,0.5,0.0,0.0,1000.,1000.,0.998,3.,1000.,0.,0.,0.,0.5,9999.,-9999.,-9999.,-9999.,1] # pp filtering
    # variables as the order they are defined in the TTree (to change)
    #    cuts_PbPb_0_8 = [-1, 0.003, 0.4, 1.5, 0.,  0.15, 0., 0.9998, 1., 0.1, 0.1, 0.1,1.5,1.5,  2.2864-0.2, 4, 0.497648-0.01,1., 0]
    #    cuts_PbPb_8_999 = [-1, 0.003, 0.4, 1.5, 0.,  0.15, 0., 0.9998, 1., 0.1, 0.1, 0.1,1.5,1.5,  2.2864-0.2, 8, 0.497648-0.01,1., 0]
    #    cuts_pp = [-1., -1, 1.5, 0.09, 0.,  0., 0., 0.99, 0.7, 0.2, 0.2, 0.1,0.09, 0.09,  2.2864-0.25, 4, 0.497648-0.009,1., 0]

print('signal dataframe:')
print(list(dataframe_sig))
print('background dataframe:')
print(list(dataframe_bkg))

def varplot(decaytype = 'LctopK0s', ptmin = 4., ptmax = 8.):
    figure = plt.figure(figsize = (20,12))
    i = 1
    print('size bkg dataframe before pt cut = ', len(dataframe_bkg.index))
    dataframe_bkg_cut = dataframe_bkg.loc[(dataframe_bkg['pt_cand'] > ptmin) & (dataframe_bkg['pt_cand'] < ptmax)]
    print('size bkg dataframe after pt cut = ', len(dataframe_bkg_cut.index))
    dataframe_sig_cut = dataframe_sig.loc[(dataframe_sig['pt_cand'] > ptmin) & (dataframe_sig['pt_cand'] < ptmax)]
    for cutpp, cutPbPb_low, cutPbPb_high, var in zip(cuts_pp, cuts_PbPb_0_8, cuts_PbPb_8_999, list_variables):
        if(var==''): 
            continue
        ax = figure.add_subplot(3,int(sum([1 for j in list_variables if j != ''])) / 3 + 1,i)
        plt.ylabel('normalised entries')
        plt.xlabel(var)
        plt.hist(dataframe_sig_cut[var], label='signal',color='b', alpha=0.5, density=True, bins=50, log=True)
        plt.hist(dataframe_bkg_cut[var], label='background',color='r', alpha=0.5, density=True, bins=50, log=True)
        if ptmin == 4.:
            plt.plot([cutPbPb_low, cutPbPb_high], [0,100],color='g', label='Pb-Pb cut')
        elif ptmin == 8.:
            plt.plot([cutPbPb_high ,cutPbPb_high], [0,100],color='g', label='Pb-Pb cut')
        plt.plot([cutpp, cutpp] , [0,100],color='y', label='pp cut')
        low = np.min(dataframe_bkg_cut[var])
        high = np.max(dataframe_bkg_cut[var])
        plt.xlim(low, high)
        plt.legend()
        i += 1

    plotname = 'distributions2_'+decaytype+'_pt'+str(int(ptmin))+'-'+str(int(ptmax))+'.png'
    plt.savefig(plotname)

# function to check signal and background efficiency after cuts
# just pKpi inserted for now
def checkcuts():

    list_variables = ['d_len', 'cos_p', 'dca', 'dist_12', 'dist_23', 'sig_vert', 'pt_prong0', 'pt_prong1', 'pt_prong2']
    cuts_pKpi = [0.00625, 0.90, 0.0375, 0.025, 0.025, 0.035, 0.5, 0.5, 0.5,2.2864-0.13,4,0,0]
    isLowerCut = [True, True, False, True, True, False, True, True, True ]
    # all events:
    #dataframe_bkg_after_cut = dataframe_bkg
    #dataframe_sig_after_cut = dataframe_sig
    # fraction of overall events to speed up:
    dataframe_bkg_after_cut = dataframe_bkg.iloc[:100000]
    dataframe_sig_after_cut = dataframe_sig.iloc[:100000]
    n_no_cut_bkg = len(dataframe_bkg_after_cut.index)
    n_no_cut_sig = len(dataframe_sig_after_cut.index)
    print('initial size bkg = ',n_no_cut_bkg)
    print('initial size sig = ',n_no_cut_sig)
    for var, cut, cutlow in zip(list_variables, cuts_pKpi, isLowerCut):

        if cutlow:
            dataframe_bkg_after_cut = dataframe_bkg_after_cut.loc[(dataframe_bkg_after_cut[var] > cut)]
            dataframe_sig_after_cut = dataframe_sig_after_cut.loc[(dataframe_sig_after_cut[var] > cut)]
        else:
            dataframe_bkg_after_cut = dataframe_bkg_after_cut.loc[(dataframe_bkg_after_cut[var] < cut)]
            dataframe_sig_after_cut = dataframe_sig_after_cut.loc[(dataframe_sig_after_cut[var] < cut)]
        print('size after cut ',var,'\t ',len(dataframe_bkg_after_cut.index))
#    dataframe_sig.loc[(dataframe_sig['pt_cand'] > ptmin) & (dataframe_sig['pt_cand'] < ptmax)

    n_after_cuts_bkg = len(dataframe_bkg_after_cut.index)
    n_after_cuts_sig = len(dataframe_sig_after_cut.index)
    print('bkg size after all cuts ', n_after_cuts_bkg)
    print('sig size after all cuts ', n_after_cuts_sig)

    eff_bkg = n_after_cuts_bkg / n_no_cut_bkg
    eff_sig = n_after_cuts_sig / n_no_cut_sig
    print('bkg efficiency of cuts = ', eff_bkg)
    print('sig efficiency of cuts = ', eff_sig)

varplot(decaytype,4.,8.)
varplot(decaytype,8.,999.)
#checkcuts()
