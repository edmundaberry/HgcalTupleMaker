import FWCore.ParameterSet.Config as cms

hgcalTuplePFCands = cms.EDProducer("HgcalTupleMaker_PFCands",
    InputTag = cms.InputTag('particleFlow'),                                  
    hgceeInputTag  = cms.InputTag('particleFlowClusterHGCEE'),
    hgchebInputTag  = cms.InputTag('particleFlowClusterHGCHEB'),
    hgchefInputTag  = cms.InputTag('particleFlowClusterHGCHEF'),
    Prefix = cms.string('PFCand'),
    Suffix = cms.string('')
)
