import FWCore.ParameterSet.Config as cms

# raw-to-digi conversion
from EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff import ctppsRawToDigi

# RP reconstruction chain with standard settings
from RecoCTPPS.Configuration.recoCTPPS_cff import recoCTPPS

from UserCode.CTPPSHLT.hltCTPPSLocalTrackFilter_cfi import hltCTPPSLocalTrackFilter

ctppsLocalTrackFilter = cms.Sequence(
  ctppsRawToDigi * recoCTPPS * hltCTPPSLocalTrackFilter
)
