import FWCore.ParameterSet.Config as cms

hgcalTupleVertex = cms.EDProducer("HgcalTupleMaker_Vertex",
    InputTag = cms.InputTag('offlinePrimaryVertices'),
    Prefix = cms.string('Vertex'),
    Suffix = cms.string('')
)
