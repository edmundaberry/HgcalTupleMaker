import FWCore.ParameterSet.Config as cms

hgcalTupleHGEERecHits = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag('HGCalRecHit','HGCEERecHits'),
    Prefix = cms.string('HGCEERecHit'),
    GeometryTag = cms.string('HGCalEESensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (True)
)

hgcalTupleHGHEBRecHits = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag('HGCalRecHit','HGCHEBRecHits'),
    Prefix = cms.string('HGCHEBRecHit'),
    GeometryTag = cms.string('HGCalHEScintillatorSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)

hgcalTupleHGHEFRecHits = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag('HGCalRecHit','HGCHEFRecHits'),
    Prefix = cms.string('HGCHEFRecHit'),
    GeometryTag = cms.string('HGCalHESiliconSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)
