import FWCore.ParameterSet.Config as cms

hgcalTuplePFCands = cms.EDProducer("HgcalTupleMaker_PFCands",
    InputTag = cms.InputTag('particleFlow'),                                  
    hgceeInputTag  = cms.InputTag('HGCalRecHit','HGCEERecHits'),
    hgchebInputTag = cms.InputTag('HGCalRecHit','HGCHEBRecHits'),
    hgchefInputTag = cms.InputTag('HGCalRecHit','HGCHEFRecHits'),
    Prefix = cms.string('PFCand'),
    Suffix = cms.string('')
)
