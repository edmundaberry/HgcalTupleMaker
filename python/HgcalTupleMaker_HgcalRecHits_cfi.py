import FWCore.ParameterSet.Config as cms

hgcalTupleHGEERecHits = cms.EDProducer("HgcalTupleMaker_HgcalRecHits",
    InputTag = cms.InputTag('HGCalRecHit','HGCEERecHits'),
    Prefix = cms.string('HGCEERecHit'),
    Suffix = cms.string('')
)

hgcalTupleHGHEBRecHits = cms.EDProducer("HgcalTupleMaker_HgcalRecHits",
    InputTag = cms.InputTag('HGCalRecHit','HGCHEBRecHits'),
    Prefix = cms.string('HGCHEBRecHit'),
    Suffix = cms.string('')
)

hgcalTupleHGHEFRecHits = cms.EDProducer("HgcalTupleMaker_HgcalRecHits",
    InputTag = cms.InputTag('HGCalRecHit','HGCHEFRecHits'),
    Prefix = cms.string('HGCHEFRecHit'),
    Suffix = cms.string('')
)
