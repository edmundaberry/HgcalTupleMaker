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
        "root://eoscms//eos/cms/store/user/eberry/HGCAL/HgcalRelVal_QQH_MC_RECO_MERGED/HgcalRelVal_QQH_MC_RECO.root"
   )
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

#------------------------------------------------------------------------------------
# Set up the output
#------------------------------------------------------------------------------------

process.TFileService = cms.Service("TFileService",
    fileName = cms.string( 'HGCAL_output.root' )
)

#------------------------------------------------------------------------------------
# Prune the jets with a smaller radius parameter
#------------------------------------------------------------------------------------

# process.load("Configuration.StandardSequences.Reconstruction_cff")
# process.ca8PFJetsCHSFinePruned = process.ca8PFJetsCHSPruned.clone()

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------

process.load("HGCALANA.HgcalTupleMaker.HgcalTupleMaker_cfi")

process.hgcalTupleTree = cms.EDAnalyzer("HgcalTupleMaker_Tree",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_hgcalTupleEvent_*_*',
        'keep *_hgcalTupleGenParticles_*_*',
        'keep *_hgcalTupleGenJets_*_*',
        'keep *_hgcalTuplePFJets_*_*',
        'keep *_hgcalTupleCaloJets_*_*',
        'keep *_hgcalTupleHGEERecHits_*_*', 
        'keep *_hgcalTupleHGHEBRecHits_*_*', 
        'keep *_hgcalTupleHGHEFRecHits_*_*'
    )
)                                        

#------------------------------------------------------------------------------------
# Set up path
#------------------------------------------------------------------------------------

process.p = cms.Path(
    process.hgcalTupleEvent*
    process.hgcalTupleGenParticles*
    process.hgcalTupleGenJets*
    process.hgcalTuplePFJets*
    process.hgcalTupleCaloJets*
    process.hgcalTupleHGEERecHits* 
    process.hgcalTupleHGHEBRecHits*
    process.hgcalTupleHGHEFRecHits*
    process.hgcalTupleTree
)

#------------------------------------------------------------------------------------
# Make a schedule and run
#------------------------------------------------------------------------------------
    
process.schedule = cms.Schedule(process.p)

