import FWCore.ParameterSet.Config as cms

hgcalTupleHGEEPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag("selectPFClusters", "HGCEEClusters"),
    HGCEERecHits  = cms.InputTag("selectHGCEEClusterRecHits" , "HGCEERecHits"),
    HGCHEFRecHits = cms.InputTag("selectHGCEEClusterRecHits" , "HGCHEFRecHits"),
    HGCHEBRecHits = cms.InputTag("selectHGCEEClusterRecHits" , "HGCHEBRecHits"),
    Prefix = cms.string('HGCEEPFCluster'),
    Suffix = cms.string('')
)

hgcalTupleHGHEBPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag("selectPFClusters", "HGCHEBClusters"),
    HGCEERecHits  = cms.InputTag("selectHGCHEBClusterRecHits" , "HGCEERecHits"),
    HGCHEFRecHits = cms.InputTag("selectHGCHEBClusterRecHits" , "HGCHEFRecHits"),
    HGCHEBRecHits = cms.InputTag("selectHGCHEBClusterRecHits" , "HGCHEBRecHits"),
    Prefix = cms.string('HGCHEBPFCluster'),
    Suffix = cms.string('')
)

hgcalTupleHGHEFPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag("selectPFClusters", "HGCHEFClusters"),
    HGCEERecHits  = cms.InputTag("selectHGCHEFClusterRecHits" , "HGCEERecHits"),
    HGCHEFRecHits = cms.InputTag("selectHGCHEFClusterRecHits" , "HGCHEFRecHits"),
    HGCHEBRecHits = cms.InputTag("selectHGCHEFClusterRecHits" , "HGCHEBRecHits"),
    Prefix = cms.string('HGCHEFPFCluster'),
    Suffix = cms.string('')
)
