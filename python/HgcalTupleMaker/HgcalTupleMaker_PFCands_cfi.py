import FWCore.ParameterSet.Config as cms

hgcalTuplePFCands = cms.EDProducer("HgcalTupleMaker_PFCands",
    InputTag = cms.InputTag('selectPFCands'),
    hgceeInputTag  = cms.InputTag('selectPFClusters', 'HGCEEClusters'),
    hgchebInputTag = cms.InputTag('selectPFClusters', 'HGCHEBClusters'),
    hgchefInputTag = cms.InputTag('selectPFClusters', 'HGCHEFClusters'),
    Prefix = cms.string('PFCand'),
    Suffix = cms.string('')
)
