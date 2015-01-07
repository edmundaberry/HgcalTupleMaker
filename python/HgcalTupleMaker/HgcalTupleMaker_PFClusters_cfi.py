import FWCore.ParameterSet.Config as cms

hgcalTupleHGEEPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag("selectPFClusters", "HGCEEClusters"),
    RecHits = cms.InputTag("selectHGCEERecHits"),
    Prefix = cms.string('HGCEEPFCluster'),
    Suffix = cms.string('')
)

hgcalTupleHGHEBPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag("selectPFClusters", "HGCHEBClusters"),
    RecHits = cms.InputTag("selectHGCHEBRecHits"),
    Prefix = cms.string('HGCHEBPFCluster'),
    Suffix = cms.string('')
)

hgcalTupleHGHEFPFClusters = cms.EDProducer("HgcalTupleMaker_PFClusters",
    InputTag = cms.InputTag("selectPFClusters", "HGCHEFClusters"),
    RecHits = cms.InputTag("selectHGCHEFRecHits"),
    Prefix = cms.string('HGCHEFPFCluster'),
    Suffix = cms.string('')
)
