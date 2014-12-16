#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

#------------------------------------------------------------------------------------
# Declare the process
#------------------------------------------------------------------------------------

process = cms.Process("ANA")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

#------------------------------------------------------------------------------------
# Set up the input
#------------------------------------------------------------------------------------

process.source = cms.Source("PoolSource", 
   fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQQH1352T_Tauola_14TeV/GEN-SIM-RECO/DES23_62_V1_HGCalV5-v1/00000/5A5B93CA-C46E-E411-AA16-002618943957.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQQH1352T_Tauola_14TeV/GEN-SIM-RECO/DES23_62_V1_HGCalV5-v1/00000/62B136A1-BF6E-E411-9F35-0026189438B3.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQQH1352T_Tauola_14TeV/GEN-SIM-RECO/DES23_62_V1_HGCalV5-v1/00000/D8E5A7A3-BF6E-E411-ABB6-00261894382D.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQQH1352T_Tauola_14TeV/GEN-SIM-RECO/DES23_62_V1_HGCalV5-v1/00000/E474D02D-B36E-E411-9673-0025905964BE.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQQH1352T_Tauola_14TeV/GEN-SIM-RECO/DES23_62_V1_HGCalV5-v1/00000/E661606D-C76E-E411-AD1E-0026189438D6.root' 
   )
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
                                    # fileName = cms.string( 'HGCAL_WJet_output.root' )
fileName = cms.string( 'HGCAL_QQH_output.root' )
)

#------------------------------------------------------------------------------------
# Set up the global tag
#------------------------------------------------------------------------------------

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

#------------------------------------------------------------------------------------
# Set up geometry to get positions of rechits:
#------------------------------------------------------------------------------------

process.load('Configuration.Geometry.GeometryExtended2023HGCalV4MuonReco_cff')

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------

process.load("HGCALANA.HgcalTupleMaker.HgcalTupleMaker.HgcalTupleMaker_cfi")

process.hgcalTupleTree = cms.EDAnalyzer("HgcalTupleMaker_Tree",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_hgcalTupleEvent_*_*',
        'keep *_hgcalTupleGenParticles_*_*',
        'keep *_hgcalTupleGenJets_*_*',
        'keep *_hgcalTuplePFJets_*_*',
        'keep *_hgcalTuplePFCA*Jets_*_*',
        'keep *_hgcalTuplePFCands_*_*',
        'keep *_hgcalTupleHGEEPFClusters_*_*',
        'keep *_hgcalTupleHGHEBPFClusters_*_*',
        'keep *_hgcalTupleHGHEFPFClusters_*_*',
        'keep *_hgcalTupleCaloJets_*_*',
        'keep *_hgcalTupleHGEERecHits_*_*', 
        'keep *_hgcalTupleHGHEBRecHits_*_*', 
        'keep *_hgcalTupleHGHEFRecHits_*_*'
    )
)                                        

#------------------------------------------------------------------------------------
# You'll need to re-run pfNoPileUpJMESequence in order to make new jet collections
# (e.g. new R parameters for CambridgeAachen and new rcut_parameters for pruning)
#------------------------------------------------------------------------------------

process.load("CommonTools.ParticleFlow.pfNoPileUpJME_cff")

#------------------------------------------------------------------------------------
# Make a list of the pruning rcut_factors you want to include and analyze
# Note that 0.5 (CMS default) is included by default
#------------------------------------------------------------------------------------

rcut_factors = [ 
    0.4  ,
    0.3  ,
    0.2  ,
    0.1  ,
    0.05 ,
    0.04 ,
    0.03 ,
    0.02 ,
    0.01 ,
    0.008,
    0.005,
    0.003,
    0.001
]

process.load("RecoParticleFlow.PFClusterProducer.particleFlowRecHitHGC_cff")

#------------------------------------------------------------------------------------
# Process the list of rcut_factors.  This makes two new sequences:
# process.myJetProducerSequence: produces jet collections with rcut_parameters applied
# process.myJetTupleMakerSequence: analyzes jet collections
#------------------------------------------------------------------------------------

from HGCALANA.HgcalTupleMaker.HgcalTupleMaker.HgcalTupleMaker_PFPrunedJets_cfi import makePrunedJetTupleMakersForRCuts

makePrunedJetTupleMakersForRCuts (process, rcut_factors ) 

#------------------------------------------------------------------------------------
# Define the final path
#------------------------------------------------------------------------------------

process.p = cms.Path(
    # Needed to get PFRecHits
    process.particleFlowRecHitHGC*
    # Needed to produce new jet collections:
    process.pfNoPileUpJMESequence*          
    # Produce new jet collections
    process.myJetProducerSequence*
    process.myPrunedJetProducerSequence*
    # Make HGCAL tuples
    process.myPrunedJetTupleMakerSequence*
    process.hgcalTupleCaloJets*
    process.hgcalTupleEvent*
    process.hgcalTupleGenParticles*
    process.hgcalTupleGenJets*
    process.hgcalTuplePFCands*
    process.hgcalTupleHGEEPFClusters*
    process.hgcalTupleHGHEBPFClusters*
    process.hgcalTupleHGHEFPFClusters*
    process.hgcalTupleHGEERecHits* 
    process.hgcalTupleHGHEBRecHits*
    process.hgcalTupleHGHEFRecHits*
    # Package everything into a tree
    process.hgcalTupleTree
)

#------------------------------------------------------------------------------------
# Make a schedule and run
#------------------------------------------------------------------------------------

process.schedule = cms.Schedule(process.p)

