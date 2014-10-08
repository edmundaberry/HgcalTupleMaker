import FWCore.ParameterSet.Config as cms

hgcalTupleHGEEPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag('particleFlowClusterHGCEE'),                                  
    RecHits = cms.InputTag('HGCalRecHit','HGCEERecHits'),
    Prefix = cms.string('HGCEEPFCluster'),
    Suffix = cms.string('')
)

hgcalTupleHGHEBPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag('particleFlowClusterHGCHEB'),                                  
    RecHits = cms.InputTag('HGCalRecHit','HGCHEBRecHits'),
    Prefix = cms.string('HGCHEBPFCluster'),
    Suffix = cms.string('')
)

hgcalTupleHGHEFPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag('particleFlowClusterHGCHEF'),                                  
    RecHits = cms.InputTag('HGCalRecHit','HGCHEFRecHits'),
    Prefix = cms.string('HGCHEFPFCluster'),
    Suffix = cms.string('')
)
