import FWCore.ParameterSet.Config as cms

selectPFCands = cms.EDProducer("HgcalTupleMaker_PFCandProducer",
    RawJetInputTag = cms.InputTag ("ca4PFJetsCHS"),
    MaxSize = cms.uint32(10)
)

selectPFClusters = cms.EDProducer("HgcalTupleMaker_HGCClusterProducer",
    RawCandInputTag = cms.InputTag ("selectPFCands")
)

selectHGCEERecHits = cms.EDProducer("HgcalTupleMaker_HGCRecHitProducer",
    RawClusterInputTag = cms.InputTag("selectPFClusters", "HGCEEClusters"),
    RecHitInputTag = cms.InputTag("HGCalRecHit","HGCEERecHits")
)

selectHGCHEFRecHits = cms.EDProducer("HgcalTupleMaker_HGCRecHitProducer",
    RawClusterInputTag = cms.InputTag("selectPFClusters", "HGCHEFClusters"),
    RecHitInputTag = cms.InputTag("HGCalRecHit","HGCHEFRecHits")
)

selectHGCHEBRecHits = cms.EDProducer("HgcalTupleMaker_HGCRecHitProducer",
    RawClusterInputTag = cms.InputTag("selectPFClusters", "HGCHEBClusters"),
    RecHitInputTag = cms.InputTag("HGCalRecHit","HGCHEBRecHits")
)

hgcPFSelectorSequence = cms.Sequence (
    selectPFCands*
    selectPFClusters*
    selectHGCEERecHits*
    selectHGCHEFRecHits*
    selectHGCHEBRecHits
)
