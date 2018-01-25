// <author>Cristovao Beirao da Cruz e Silva</author>
// <email>cbeiraod@cern.ch</email>
// <created>2017-10-26</created>
// <description>
// </description>

#include "UserCode/CTPPSHLT/interface/HLTCTPPSLocalTrackFilter.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Handle.h"

#include "HLTrigger/HLTcore/interface/defaultModuleLabel.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"


// Fill descriptions, for CMSSW
void HLTCTPPSLocalTrackFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  makeHLTFilterDescription(desc);

  desc.add<edm::InputTag>("pixelLocalTrackInputTag",   edm::InputTag("ctppsPixelLocalTracks"))
    ->setComment("input tag of the pixel local track collection");
  desc.add<edm::InputTag>("stripLocalTrackInputTag",   edm::InputTag("totemRPLocalTrackFitter"))
    ->setComment("input tag of the strip local track collection");
  desc.add<edm::InputTag>("diamondLocalTrackInputTag", edm::InputTag("ctppsDiamondLocalTracks"))
    ->setComment("input tag of the diamond local track collection");

  desc.add<unsigned int>("detectorBitset", static_cast<unsigned int>(1)) // TODO: Consider changing to 5 once the diamonds are implemented
    ->setComment("bitset of which detector types to consider: bit 1 -> pixel; bit 2 -> strips; bit 3 -> diamonds. eg. the value 1 will only consider the pixel detectors, the value 5 will consider the diamond and pixel detectors");

  desc.add<int>("minTracks", 2)
    ->setComment("minimum number of tracks");
  desc.add<int>("minTracksPerArm", 1)
    ->setComment("minimum number of tracks per arm of the CTPPS detector");

  desc.add<int>("triggerType", trigger::TriggerTrack);

  descriptions.add("hltCTPPSLocalTrackFilter", desc);
  return;
}


// Destructor
HLTCTPPSLocalTrackFilter::~HLTCTPPSLocalTrackFilter()= default;

// Constructor
HLTCTPPSLocalTrackFilter::HLTCTPPSLocalTrackFilter(const edm::ParameterSet& iConfig):
  HLTFilter(iConfig),
  pixelLocalTrackInputTag_   (iConfig.getParameter< edm::InputTag > ("pixelLocalTrackInputTag")),
  stripLocalTrackInputTag_   (iConfig.getParameter< edm::InputTag > ("stripLocalTrackInputTag")),
  diamondLocalTrackInputTag_ (iConfig.getParameter< edm::InputTag > ("diamondLocalTrackInputTag")),
  detectorBitset_            (iConfig.getParameter< unsigned int  > ("detectorBitset")),
  minTracks_                 (iConfig.getParameter< int           > ("minTracks")),
  minTracksPerArm_           (iConfig.getParameter< int           > ("minTracksPerArm")),
  triggerType_               (iConfig.getParameter< int           > ("triggerType")),
  usePixel_   (false),
  useStrip_   (false),
  useDiamond_ (false)
{
  if(detectorBitset_ | 1)
    usePixel_ = true;
  if(detectorBitset_ | 2)
    useStrip_ = true;
  if(detectorBitset_ | 4)
    useDiamond_ = true;

  if(usePixel_)
    pixelLocalTrackToken_   = consumes<edm::DetSetVector<CTPPSPixelLocalTrack>>(pixelLocalTrackInputTag_);
  if(useStrip_)
    stripLocalTrackToken_   = consumes<edm::DetSetVector<TotemRPLocalTrack>>(stripLocalTrackInputTag_);
  if(useDiamond_)
    diamondLocalTrackToken_ = consumes<edm::DetSetVector<CTPPSDiamondLocalTrack>>(diamondLocalTrackInputTag_);

  LogDebug("") << "HLTCTPPSLocalTrackFilter: pixelTag/stripTag/diamondTag/bitset/minTracks/minTracksPerArm/triggerType : "
               << pixelLocalTrackInputTag_.encode() << " "
               << stripLocalTrackInputTag_.encode() << " "
               << diamondLocalTrackInputTag_.encode() << " "
               << detectorBitset_ << " "
               << minTracks_ << " "
               << minTracksPerArm_ << " "
               << triggerType_;
}

// Filter events, for triggering
bool HLTCTPPSLocalTrackFilter::hltFilter(edm::Event& iEvent, const edm::EventSetup& iSetup, trigger::TriggerFilterObjectWithRefs& filterproduct) const
{
  int arm45Tracks = 0;
  int arm56Tracks = 0;

  if (saveTags())
  {
    filterproduct.addCollectionTag(pixelLocalTrackInputTag_);
    filterproduct.addCollectionTag(stripLocalTrackInputTag_);
    filterproduct.addCollectionTag(diamondLocalTrackInputTag_);
  }

  typedef edm::Ref<edm::DetSetVector<CTPPSPixelLocalTrack>> PixelRef;
  typedef edm::Ref<edm::DetSetVector<TotemRPLocalTrack>> StripRef;
  typedef edm::Ref<edm::DetSetVector<CTPPSDiamondLocalTrack>> DiamondRef;

  //   Note that there is no matching between the tracks of the several detectors
  // so tracks from separate detectors might correspond to the same particle.
  // When/If the pixels are used in more than one RP, then the same situation can
  // happen within the pixels themselves. To be seen...
  if(usePixel_)
  {
    edm::Handle<edm::DetSetVector<CTPPSPixelLocalTrack>> pixelTracks;
    iEvent.getByToken(pixelLocalTrackToken_, pixelTracks);

    for(const auto &rpv : (*pixelTracks))
    {
      CTPPSPixelDetId id(rpv.id);

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          if(id.arm() == 0) ++arm45Tracks;
          if(id.arm() == 1) ++arm56Tracks;
        }
      }
      // Still not able to get the line below to work, but it seems no one keeps DetSets, so it should be fine as is
      //filterproduct.addObject(trigger::TriggerTrack, PixelRef(pixelTracks, rpv.id));
    }
  }

  if(useStrip_)
  {
    edm::Handle<edm::DetSetVector<TotemRPLocalTrack>> stripTracks;
    iEvent.getByToken(stripLocalTrackToken_, stripTracks);

    for(const auto &rpv : (*stripTracks))
    {
      TotemRPDetId id(rpv.id);

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          if(id.arm() == 0) ++arm45Tracks;
          if(id.arm() == 1) ++arm56Tracks;
        }
      }
      // Still not able to get the line below to work, but it seems no one keeps DetSets, so it should be fine as is
      //filterproduct.addObject(trigger::TriggerTrack, StripRef(stripTracks, rpv.id));
    }
  }

  if(useDiamond_)
  {
    edm::Handle<edm::DetSetVector<CTPPSDiamondLocalTrack>> diamondTracks;
    iEvent.getByToken(diamondLocalTrackToken_, diamondTracks);

    for(const auto &rpv : (*diamondTracks))
    {
      CTPPSDiamondDetId id(rpv.id);

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          if(id.arm() == 0) ++arm45Tracks;
          if(id.arm() == 1) ++arm56Tracks;
        }
      }
      // Still not able to get the line below to work, but it seems no one keeps DetSets, so it should be fine as is
      //filterproduct.addObject(trigger::TriggerTrack, StripRef(diamondTracks, rpv.id));
    }
  }


  bool accept = true;

  if(arm45Tracks + arm56Tracks < minTracks_ || arm45Tracks < minTracksPerArm_ || arm56Tracks < minTracksPerArm_)
    accept = false;

  return accept;
}
