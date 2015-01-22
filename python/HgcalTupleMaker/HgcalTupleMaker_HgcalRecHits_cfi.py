import FWCore.ParameterSet.Config as cms

hgcalTupleHGCEERecHitsFromHGCEEClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCEEClusterRecHits", "HGCEERecHits"),
    Prefix = cms.string('HGCEERecHitFromHGCEECluster'),
    GeometryTag = cms.string('HGCalEESensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (True)
)

hgcalTupleHGCHEBRecHitsFromHGCEEClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCEEClusterRecHits", "HGCHEBRecHits"),
    Prefix = cms.string('HGCHEBRecHitFromHGCEECluster'),
    GeometryTag = cms.string('HGCalHEScintillatorSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)

hgcalTupleHGCHEFRecHitsFromHGCEEClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCEEClusterRecHits", "HGCHEFRecHits"),
    Prefix = cms.string('HGCHEFRecHitFromHGCEECluster'),
    GeometryTag = cms.string('HGCalHESiliconSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)

hgcalTupleHGCEERecHitsFromHGCHEFClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCHEFClusterRecHits", "HGCEERecHits"),
    Prefix = cms.string('HGCEERecHitFromHGCHEFCluster'),
    GeometryTag = cms.string('HGCalEESensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (True)
)

hgcalTupleHGCHEBRecHitsFromHGCHEFClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCHEFClusterRecHits", "HGCHEBRecHits"),
    Prefix = cms.string('HGCHEBRecHitFromHGCHEFCluster'),
    GeometryTag = cms.string('HGCalHEScintillatorSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)

hgcalTupleHGCHEFRecHitsFromHGCHEFClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCHEFClusterRecHits", "HGCHEFRecHits"),
    Prefix = cms.string('HGCHEFRecHitFromHGCHEFCluster'),
    GeometryTag = cms.string('HGCalHESiliconSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)

hgcalTupleHGCEERecHitsFromHGCHEBClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCHEBClusterRecHits", "HGCEERecHits"),
    Prefix = cms.string('HGCEERecHitFromHGCHEBCluster'),
    GeometryTag = cms.string('HGCalEESensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (True)
)

hgcalTupleHGCHEBRecHitsFromHGCHEBClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCHEBClusterRecHits", "HGCHEBRecHits"),
    Prefix = cms.string('HGCHEBRecHitFromHGCHEBCluster'),
    GeometryTag = cms.string('HGCalHEScintillatorSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)

hgcalTupleHGCHEFRecHitsFromHGCHEBClusters = cms.EDProducer('HgcalTupleMaker_HgcalRecHits',
    InputTag = cms.InputTag("selectHGCHEBClusterRecHits", "HGCHEFRecHits"),
    Prefix = cms.string('HGCHEFRecHitFromHGCHEBCluster'),
    GeometryTag = cms.string('HGCalHESiliconSensitive'),
    Suffix = cms.string(''),
    isHGEE = cms.bool (False)
)
