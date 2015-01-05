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
       #FILENAMES
   )
)

process.source.skipEvents = cms.untracked.uint32(
    #SKIPEVENTS
)


process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(
       #PROCESSEVENTS
   )
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
                                    # fileName = cms.string( 'HGCAL_WJet_output.root' )
fileName = cms.string( 'OUTPUTFILENAME.root')
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
        'keep *_hgcalTuplePFTaus_*_*',
        'keep *_hgcalTuplePFCands_*_*',
        'keep *_hgcalTupleHGEEPFClusters_*_*',
        'keep *_hgcalTupleHGHEBPFClusters_*_*',
        'keep *_hgcalTupleHGHEFPFClusters_*_*',
        'keep *_hgcalTupleCaloJets_*_*',
        'keep *_hgcalTupleHGEERecHits_*_*', 
        'keep *_hgcalTupleHGHEBRecHits_*_*', 
        'keep *_hgcalTupleHGHEFRecHits_*_*',
        'keep *_hgcalTupleVertex_*_*',
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
    process.hgcalTuplePFTaus*
    process.hgcalTuplePFCands*
    process.hgcalTupleHGEEPFClusters*
    process.hgcalTupleHGHEBPFClusters*
    process.hgcalTupleHGHEFPFClusters*
    process.hgcalTupleHGEERecHits* 
    process.hgcalTupleHGHEBRecHits*
    process.hgcalTupleHGHEFRecHits*
    process.hgcalTupleVertex*
    # Package everything into a tree
    process.hgcalTupleTree
)

#------------------------------------------------------------------------------------
# Make a schedule and run
#------------------------------------------------------------------------------------

process.schedule = cms.Schedule(process.p)

