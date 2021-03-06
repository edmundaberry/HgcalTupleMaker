cmsDriver.py step3 \
--conditions auto:upgradePLS3 \
-n 10 \
--eventcontent FEVTDEBUGHLT \
-s RAW2DIGI,L1Reco,RECO \
--datatier GEN-SIM-RECO \
--customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2023HGCalMuon \
--geometry Extended2023HGCalMuon,Extended2023HGCalMuonReco \
--magField 38T_PostLS1 \
--pileup AVE_140_BX_25ns \
--filein file:step2.root \
--fileout file:step3.root \
--no_exec
