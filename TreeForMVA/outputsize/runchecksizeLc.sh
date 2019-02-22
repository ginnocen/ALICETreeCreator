#!/bin/bash
g++ CheckTreeSize.C $(root-config --cflags --libs) -g -o CheckTreeSize.exe
g++ DrawHistoFromTrees.C $(root-config --cflags --libs) -g -o DrawHistoFromTrees.exe

#enum cutopt {kLowerLimit,kUpperLimit,kUpperLimitAbsValue};
#./CheckTreeSize.exe "10" "6" "5" "9999" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "ctau_K0s_ML" "100" "20" "5" "1" "ctau_K0s_ML.root"

./DrawHistoFromTrees.exe "0" "1000000000" "6" "5" "9999" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "ctau_K0s_ML" "100" "5" "50" "1" "test_ctau_K0s_ML_5-9999.root"
#./CheckTreeSize.exe "0" "1000000000" "6" "5" "9999" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "ctau_K0s_ML" "100" "5" "50" "1" "test_ctau_K0s_ML_5-9999.root" & 
#./CheckTreeSize.exe "1" "1000000000" "6" "5" "9999" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "imp_par_K0s_ML" "10" "0.5" "0.5" "2" "imp_par_K0s_ML_5-9999.root" &
#./CheckTreeSize.exe "2" "1000000000" "6" "5" "9999" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "cos_p_K0s_ML" "0.99" "0.999" "0.0005" "0" "cos_p_K0s_ML_5-9999.root" &

#./CheckTreeSize.exe "0" "1000000000" "6" "0" "5" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "ctau_K0s_ML" "100" "5" "5" "1" "ctau_K0s_ML_0-5.root" & 
#./CheckTreeSize.exe "1" "1000000000" "6" "0" "5" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "imp_par_K0s_ML" "10" "0.5" "0.5" "2" "imp_par_K0s_ML_0-5.root" &
#./CheckTreeSize.exe "2" "1000000000" "6" "0" "5" "~/MachineLearningHF/ProcessOutputTTreeMVA/11-02-2019/mergedDataMC_AnalysisResults_LctopK0s_skimmed.root" "cos_p_K0s_ML" "0.99" "0.999" "0.0005" "0" "cos_p_K0s_ML_0-5.root" &

#echo '.x CheckTreeSize.C+ (1.e+7,4,0,50,"skimmedLc_a.root","cos_p_ML",0.85,0.95,0.01,kLowerLimit,"cos_p_scan.root")' | root -l
#echo '.q'
#echo '.x CheckTreeSize.C+ (1.e+7,4,0,50,"skimmedLc_a.root","d_len_ML",0.02,0.05,0.01,kLowerLimit,"d_len_scan.root")' | root -l
#echo '.q'
#echo '.x CheckTreeSize.C+ (4,0,50,"../../merge/skimmedDs.root","norm_dl_xy_ML",0.,5.,0.5,kLowerLimit,"norm_dl_xy_scan.root")' | root -l
#echo '.q'
#echo '.x CheckTreeSize.C+ (4,0,50,"../../merge/skimmedDs.root","mass_KK_ML",0.030,0.010,0.002,kUpperLimit,"delta_mass_KK_scan.root")' | root -l
#echo '.q'
#echo '.x CheckTreeSize.C+ (1.e+7,4,0,50,"skimmedLc_a.root","sig_vert_ML",0.035,0.025,0.002,kUpperLimit,"sig_vert_scan.root")' | root -l
#echo '.q'
#echo '.x CheckTreeSize.C+ (4,0,50,"../../merge/skimmedDs.root","nsigTPC_K_1_ML",5,2,0.5,kUpperLimitAbsValue,"nsigTPC_K_1_scan.root")' | root -l
#echo '.q'
#echo '.x CheckTreeSize.C+ (4,0,50,"../../merge/skimmedDs.root","nsigTOF_K_1_ML",5,2,0.5,kUpperLimitAbsValue,"nsigTOF_K_1_scan.root")' | root -l
#echo '.q'
