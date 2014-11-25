cmsDriver.py step2 \
--conditions auto:upgradePLS3 \
-n 1 \
--eventcontent FEVTDEBUGHLT \
-s DIGI:pdigi_valid,L1,DIGI2RAW \
--datatier GEN-SIM-DIGI-RAW \
--customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2023HGCal \
--geometry Extended2023HGCal,Extended2023HGCalReco \
--magField 38T_PostLS1 \
--pileup NoPileUp \
--filein file:step1.root \
--fileout file:step2.root \
--no_exec