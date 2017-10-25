import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/data/Run2017C/JetHT/RAW/v1/000/299/368/00000/00C1FD8A-ED6B-E711-AFC4-02163E01A51A.root'
    )
)

from RecoCTPPS.TotemRPLocal.totemRPLocalReconstruction_cff import *
from RecoCTPPS.TotemRPLocal.ctppsDiamondLocalReconstruction_cff import *
from RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cff import ctppsLocalTrackLiteProducer
from RecoCTPPS.PixelLocal.ctppsPixelLocalReconstruction_cff import *

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

#recoCTPPS = cms.Sequence(
#    totemRPLocalReconstruction *
#    ctppsDiamondLocalReconstruction *
#    ctppsPixelLocalReconstruction *
#    ctppsLocalTrackLiteProducer
#)

process.ctppsInspect = cms.EDAnalyzer('CTPPSInspect'
)


process.p = cms.Path(process.ctppsRawToDigi * process.recoCTPPS * process.ctppsInspect)
