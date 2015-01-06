import FWCore.ParameterSet.Config as cms

hgcalTupleHGEERecHits = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag('selectHGCEERecHits'),
    Prefix = cms.string('HGCEERecHit'),
    GeometryTag = cms.string('HGCalEESensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (True)
)

hgcalTupleHGHEBRecHits = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag('selectHGCHEBRecHits'),
    Prefix = cms.string('HGCHEBRecHit'),
    GeometryTag = cms.string('HGCalHEScintillatorSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)

hgcalTupleHGHEFRecHits = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag('selectHGCHEFRecHits'),
    Prefix = cms.string('HGCHEFRecHit'),
    GeometryTag = cms.string('HGCalHESiliconSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)
