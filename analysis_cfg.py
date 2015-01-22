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
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

#------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------

options = VarParsing.VarParsing()

options.register('skipEvents',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")

options.register('processEvents',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to process")

options.register('inputFiles',
                 "file:inputFile.root", #default value
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "file:outputFile.root", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.parseArguments()

print "Skip events =", options.skipEvents
print "Process events =", options.processEvents
print "inputFiles =", options.inputFiles
print "outputFile =", options.outputFile

#------------------------------------------------------------------------------------
# Set up the input
#------------------------------------------------------------------------------------

process.source = cms.Source("PoolSource", 
   fileNames = cms.untracked.vstring(
       options.inputFiles
   ),
   skipEvents = cms.untracked.uint32(
       options.skipEvents
   )
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(
       options.processEvents
   )
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( options.outputFile )
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

process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')

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
        'keep *_hgcalTuplePFAK*Jets_*_*',
        'keep *_hgcalTuplePFTaus_*_*',
        'keep *_hgcalTuplePFCands_*_*',
        'keep *_hgcalTupleHGEEPFClusters_*_*',
        'keep *_hgcalTupleHGHEBPFClusters_*_*',
        'keep *_hgcalTupleHGHEFPFClusters_*_*',
        'keep *_hgcalTupleCaloJets_*_*',
        'keep *_hgcalTupleHGCEERecHitsFromHGCEEClusters_*_*', 
        'keep *_hgcalTupleHGCHEBRecHitsFromHGCEEClusters_*_*', 
        'keep *_hgcalTupleHGCHEFRecHitsFromHGCEEClusters_*_*', 
        'keep *_hgcalTupleHGCEERecHitsFromHGCHEBClusters_*_*', 
        'keep *_hgcalTupleHGCHEBRecHitsFromHGCHEBClusters_*_*', 
        'keep *_hgcalTupleHGCHEFRecHitsFromHGCHEBClusters_*_*', 
        'keep *_hgcalTupleHGCEERecHitsFromHGCHEFClusters_*_*', 
        'keep *_hgcalTupleHGCHEBRecHitsFromHGCHEFClusters_*_*', 
        'keep *_hgcalTupleHGCHEFRecHitsFromHGCHEFClusters_*_*', 
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
    # Select PF objects
    process.hgcPFSelectorSequence*
    # Produce new pruned jet collections
    process.myPrunedJetProducerSequence*
    # Make HGCAL tuples
    process.myPrunedJetTupleMakerSequence*
    process.hgcalTuplePFAK1p5Jets*
    process.hgcalTupleCaloJets*
    process.hgcalTupleEvent*
    process.hgcalTupleGenParticles*
    process.hgcalTupleGenJets*
    process.hgcalTuplePFTaus*
    process.hgcalTuplePFCands*
    process.hgcalTupleHGEEPFClusters*
    process.hgcalTupleHGHEBPFClusters*
    process.hgcalTupleHGHEFPFClusters*
    process.hgcalTupleHGCEERecHitsFromHGCEEClusters* 
    process.hgcalTupleHGCHEBRecHitsFromHGCEEClusters* 
    process.hgcalTupleHGCHEFRecHitsFromHGCEEClusters* 
    process.hgcalTupleHGCEERecHitsFromHGCHEBClusters* 
    process.hgcalTupleHGCHEBRecHitsFromHGCHEBClusters* 
    process.hgcalTupleHGCHEFRecHitsFromHGCHEBClusters* 
    process.hgcalTupleHGCEERecHitsFromHGCHEFClusters* 
    process.hgcalTupleHGCHEBRecHitsFromHGCHEFClusters* 
    process.hgcalTupleHGCHEFRecHitsFromHGCHEFClusters* 
    process.hgcalTupleVertex*
    # Package everything into a tree
    process.hgcalTupleTree
)

#------------------------------------------------------------------------------------
# Make a schedule and run
#------------------------------------------------------------------------------------

process.schedule = cms.Schedule(process.p)

