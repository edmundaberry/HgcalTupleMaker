import FWCore.ParameterSet.Config as cms

selectPFCands = cms.EDProducer("HgcalTupleMaker_PFCandProducer",
    RawJetInputTag = cms.InputTag ("ca4PFJetsCHS"),
    MaxSize = cms.uint32(10)
)

selectPFClusters = cms.EDProducer("HgcalTupleMaker_HGCClusterProducer",
    RawCandInputTag    = cms.InputTag ("selectPFCands")
)

selectHGCEEClusterRecHits = cms.EDProducer("HgcalTupleMaker_HGCRecHitProducer",
    RawClusterInputTag   = cms.InputTag("selectPFClusters", "HGCEEClusters"),
    HGCEERecHitInputTag  = cms.InputTag("HGCalRecHit"     , "HGCEERecHits"),
    HGCHEFRecHitInputTag = cms.InputTag("HGCalRecHit"     , "HGCHEFRecHits"),
    HGCHEBRecHitInputTag = cms.InputTag("HGCalRecHit"     , "HGCHEBRecHits")
)

selectHGCHEFClusterRecHits = cms.EDProducer("HgcalTupleMaker_HGCRecHitProducer",
    RawClusterInputTag = cms.InputTag("selectPFClusters", "HGCHEFClusters"),
    HGCEERecHitInputTag  = cms.InputTag("HGCalRecHit"     , "HGCEERecHits"),
    HGCHEFRecHitInputTag = cms.InputTag("HGCalRecHit"     , "HGCHEFRecHits"),
    HGCHEBRecHitInputTag = cms.InputTag("HGCalRecHit"     , "HGCHEBRecHits")
)

selectHGCHEBClusterRecHits = cms.EDProducer("HgcalTupleMaker_HGCRecHitProducer",
    RawClusterInputTag = cms.InputTag("selectPFClusters", "HGCHEBClusters"),
    HGCEERecHitInputTag  = cms.InputTag("HGCalRecHit"     , "HGCEERecHits"),
    HGCHEFRecHitInputTag = cms.InputTag("HGCalRecHit"     , "HGCHEFRecHits"),
    HGCHEBRecHitInputTag = cms.InputTag("HGCalRecHit"     , "HGCHEBRecHits")
)

hgcPFSelectorSequence = cms.Sequence (
    selectPFCands*
    selectPFClusters*
    selectHGCEEClusterRecHits*
    selectHGCHEFClusterRecHits*
    selectHGCHEBClusterRecHits
)
