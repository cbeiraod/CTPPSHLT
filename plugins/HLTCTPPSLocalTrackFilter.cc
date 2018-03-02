// <author>Cristovao Beirao da Cruz e Silva</author>
// <email>cbeiraod@cern.ch</email>
// <created>2017-10-26</created>
// <description>
// </description>

#include "UserCode/CTPPSHLT/interface/HLTCTPPSLocalTrackFilter.h"

class HLTCTPPSLocalTrackFilter : public HLTFilter
{
public:
  explicit HLTCTPPSLocalTrackFilter(const edm::ParameterSet&);
  ~HLTCTPPSLocalTrackFilter() override;

  static void fillDescriptions(edm::ConfigurationDescriptions&);
  bool hltFilter(edm::Event&, const edm::EventSetup&, trigger::TriggerFilterObjectWithRefs&) const override;

private:
  edm::ParameterSet param_;

  edm::InputTag pixelLocalTrackInputTag_; // Input tag identifying the pixel detector
  edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelLocalTrack>> pixelLocalTrackToken_;

  edm::InputTag stripLocalTrackInputTag_; // Input tag identifying the strip detector
  edm::EDGetTokenT<edm::DetSetVector<TotemRPLocalTrack>> stripLocalTrackToken_;

  edm::InputTag diamondLocalTrackInputTag_; // Input tag identifying the diamond detector
  edm::EDGetTokenT<edm::DetSetVector<CTPPSDiamondLocalTrack>> diamondLocalTrackToken_;

  unsigned int detectorBitset_;

  int minTracks_;
  int minTracksPerArm_;

  int maxTracks_;
  int maxTracksPerArm_;
  int maxTracksPerPot_;

  bool usePixel_;
  bool useStrip_;
  bool useDiamond_;

protected:
};

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

  desc.add<int>("maxTracks", -1)
    ->setComment("maximum number of tracks, if smaller than minTracks it will be ignored");
  desc.add<int>("maxTracksPerArm", -1)
    ->setComment("maximum number of tracks per arm of the CTPPS detector, if smaller than minTrackPerArm it will be ignored");
  desc.add<int>("maxTracksPerPot", -1)
    ->setComment("maximum number of tracks per roman pot of the CTPPS detector, if negative it will be ignored");

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
  maxTracks_                 (iConfig.getParameter< int           > ("maxTracks")),
  maxTracksPerArm_           (iConfig.getParameter< int           > ("maxTracksPerArm")),
  maxTracksPerPot_           (iConfig.getParameter< int           > ("maxTracksPerPot")),
  usePixel_   (false),
  useStrip_   (false),
  useDiamond_ (false)
{
  if(detectorBitset_ & 1)
    usePixel_ = true;
  if(detectorBitset_ & 2)
    useStrip_ = true;
  if(detectorBitset_ & 4)
    useDiamond_ = true;

  if(usePixel_)
    pixelLocalTrackToken_   = consumes<edm::DetSetVector<CTPPSPixelLocalTrack>>(pixelLocalTrackInputTag_);
  if(useStrip_)
    stripLocalTrackToken_   = consumes<edm::DetSetVector<TotemRPLocalTrack>>(stripLocalTrackInputTag_);
  if(useDiamond_)
    diamondLocalTrackToken_ = consumes<edm::DetSetVector<CTPPSDiamondLocalTrack>>(diamondLocalTrackInputTag_);

  LogDebug("") << "HLTCTPPSLocalTrackFilter: pixelTag/stripTag/diamondTag/bitset/minTracks/minTracksPerArm : "
               << pixelLocalTrackInputTag_.encode() << " "
               << stripLocalTrackInputTag_.encode() << " "
               << diamondLocalTrackInputTag_.encode() << " "
               << detectorBitset_ << " "
               << minTracks_ << " "
               << minTracksPerArm_ << " "
               << maxTracks_ << " "
               << maxTracksPerArm_;
}

// Filter events, for triggering
bool HLTCTPPSLocalTrackFilter::hltFilter(edm::Event& iEvent, const edm::EventSetup& iSetup, trigger::TriggerFilterObjectWithRefs& filterproduct) const
{
  int arm45Tracks = 0;
  int arm56Tracks = 0;
  std::map<uint32_t, int> tracksPerPot;

  if (saveTags())
  {
    if(usePixel_)   filterproduct.addCollectionTag(pixelLocalTrackInputTag_);
    if(useStrip_)   filterproduct.addCollectionTag(stripLocalTrackInputTag_);
    if(useDiamond_) filterproduct.addCollectionTag(diamondLocalTrackInputTag_);
  }

  typedef edm::Ref<edm::DetSetVector<CTPPSPixelLocalTrack>> PixelRef;
  typedef edm::Ref<edm::DetSetVector<TotemRPLocalTrack>> StripRef;
  typedef edm::Ref<edm::DetSetVector<CTPPSDiamondLocalTrack>> DiamondRef;

  //   Note that there is no matching between the tracks from the several roman pots
  // so tracks from separate pots might correspond to the same particle.
  // When the pixels are used in more than one RP (in 2018), then the same situation can
  // happen within the pixels themselves.
  if(usePixel_) // Pixels correspond to RP 220 in 2017 data
  {
    edm::Handle<edm::DetSetVector<CTPPSPixelLocalTrack>> pixelTracks;
    iEvent.getByToken(pixelLocalTrackToken_, pixelTracks);

    for(const auto &rpv : (*pixelTracks))
    {
      CTPPSPixelDetId id(rpv.id);
      if(tracksPerPot.count(rpv.id) == 0)
        tracksPerPot[rpv.id] = 0;

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          if(id.arm() == 0) ++arm45Tracks;
          if(id.arm() == 1) ++arm56Tracks;
          tracksPerPot[rpv.id]++;
        }
      }
      // Still not able to get the line below to work, but it seems no one keeps DetSets, so it should be fine as is
      //filterproduct.addObject(trigger::TriggerTrack, PixelRef(pixelTracks, rpv.id));
    }
  }

  if(useStrip_) // Strips correspond to RP 210 in 2017 data
  {
    edm::Handle<edm::DetSetVector<TotemRPLocalTrack>> stripTracks;
    iEvent.getByToken(stripLocalTrackToken_, stripTracks);

    for(const auto &rpv : (*stripTracks))
    {
      TotemRPDetId id(rpv.id);
      if(tracksPerPot.count(rpv.id) == 0)
        tracksPerPot[rpv.id] = 0;

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          if(id.arm() == 0) ++arm45Tracks;
          if(id.arm() == 1) ++arm56Tracks;
          tracksPerPot[rpv.id]++;
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
      if(tracksPerPot.count(rpv.id) == 0)
        tracksPerPot[rpv.id] = 0;

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          if(id.arm() == 0) ++arm45Tracks;
          if(id.arm() == 1) ++arm56Tracks;
          tracksPerPot[rpv.id]++;
        }
      }
      // Still not able to get the line below to work, but it seems no one keeps DetSets, so it should be fine as is
      //filterproduct.addObject(trigger::TriggerTrack, StripRef(diamondTracks, rpv.id));
    }
  }


  bool accept = true;

  if(arm45Tracks + arm56Tracks < minTracks_ || arm45Tracks < minTracksPerArm_ || arm56Tracks < minTracksPerArm_)
    accept = false;

  if(maxTracks_ >= minTracks_ && arm45Tracks + arm56Tracks > maxTracks_)
    accept = false;

  if(maxTracksPerArm_ >= minTracksPerArm_ && (arm45Tracks > maxTracksPerArm_ || arm56Tracks > maxTracksPerArm_))
    accept = false;

  if(maxTracksPerPot_ >= 0)
  {
    for(auto& pot : tracksPerPot)
    {
      if(pot.second > maxTracksPerPot_)
      {
        accept = false;
        break;
      }
    }
  }

  return accept;
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HLTCTPPSLocalTrackFilter);
