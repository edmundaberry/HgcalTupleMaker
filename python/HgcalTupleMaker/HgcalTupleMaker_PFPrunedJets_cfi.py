import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoPFJets_cff import ca8PFJetsCHSPruned

ca4PFJetsCHSPrunedRCut5 = ca8PFJetsCHSPruned.clone( rParam = 0.4 )

hgcalTuplePFCA4PrunedRCut5Jets = cms.EDProducer("HgcalTupleMaker_PFJets",
    InputTag = cms.InputTag("ca4PFJetsCHSPrunedRCut5"),
    PFCandInputTag = cms.InputTag('particleFlow'),                                  
    Prefix = cms.string('PFCA4PrunedJetRCut5'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)
                       
myPrunedJetProducerSequence   = cms.Sequence ( ca4PFJetsCHSPrunedRCut5 ) 
myPrunedJetTupleMakerSequence = cms.Sequence ( hgcalTuplePFCA4PrunedRCut5Jets ) 

def makePrunedJetTupleMakersForRCuts ( process, rcut_factors ) :

    oldJetProducerName = "ca4PFJetsCHSPrunedRCut5"
    process.MessageLogger.suppressError =  cms.untracked.vstring( "ca4PFJetsCHS", oldJetProducerName )
    
    for rcut_factor in rcut_factors:   
        
        # Associate the rcut_factor (float) with a string
        factorString = str(rcut_factor).replace("0.","")

        # Create the new jet producer module from the old jet producer module
        newJetProducerName = oldJetProducerName.replace("5", factorString)
        setattr(process, newJetProducerName, ca4PFJetsCHSPrunedRCut5.clone() )

        # Set the new jet producer module's parameters
        newJetProducerModule = getattr( process, newJetProducerName )
        newJetProducerModule.rcut_factor = rcut_factor
        
        # Put the new jet producer into the sequence
        myPrunedJetProducerSequence.replace ( getattr(process, oldJetProducerName), 
                                              getattr(process, oldJetProducerName) + getattr(process,newJetProducerName))
        
        # Create the new hgcalTupleMaker module from the old hgcalTupleMaker module
        oldTupleMakerName = "hgcalTuplePFCA4PrunedRCut5Jets"
        newTupleMakerName = oldTupleMakerName.replace("5", factorString)
        setattr(process, newTupleMakerName,  hgcalTuplePFCA4PrunedRCut5Jets.clone() )
        
        # Set the new hgcalTupleMaker module's parameters
        newTupleMakerModule = getattr( process, newTupleMakerName ) 
        newTupleMakerModule.InputTag = cms.InputTag("ca4PFJetsCHSPrunedRCut" + factorString, "SubJets")
        newTupleMakerModule.Prefix   = cms.string  ("PFCA4PrunedJetRCut" + factorString )
    
        # Put the new hgcalTupleMaker into the sequence 
        myPrunedJetTupleMakerSequence.replace ( getattr(process, oldTupleMakerName), 
                                                getattr(process, oldTupleMakerName) + getattr(process,newTupleMakerName))
        
    
        # Silence message logger
        process.MessageLogger.suppressError.append ( newJetProducerName )
