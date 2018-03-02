import os,subprocess,ROOT
import pickle
import re
import time
import shutil

def assure_path_exists(path):
  dir = os.path.dirname(path)
  if not os.path.exists(dir):
    os.makedirs(dir)

if __name__ == "__main__":
  import argparse
  import os
  import sys

  parser = argparse.ArgumentParser(description='Process the command line options')
  parser.add_argument('-d', '--dryRun', action='store_true', help='Do a dry run (i.e. do not actually run the potentially dangerous commands but print them to the screen)')
  parser.add_argument('-v', '--verbose', action='store_true', help='Whether to print verbose output')
  parser.add_argument('-o', '--outDirectory', required=True, help='Name of the output directory')
  parser.add_argument('-i', '--inputFile', required=True, help='Name of the input file')

  args = parser.parse_args()

  toPlot = {
    "all": "ctppsAll",
    "coinc": "ctppsCoincidince",
    "l1": "ctppsCoincidenceL1Double",
    "full": "ctppsDiPFJetAve260CoincidenceDouble",
  }

  plots = {
    "Arm1Xi": {
      "name": "h_arm1xi",
      "logy": False,
      "logz": False,
    },
    "Arm2Xi": {
      "name": "h_arm2xi",
      "logy": False,
      "logz": False,
    },
  }

  file = ROOT.TFile.Open(args.inputFile, "READ")
  assure_path_exists(args.outDirectory + "/dummy")
  if args.verbose:
    print file

  for condition in toPlot:
    plotDirectory = file.Get(toPlot[condition])
    if args.verbose:
      print "  ", plotDirectory
    for plotName in plots:
      plot = plotDirectory.Get(plots[plotName]["name"])
      if args.verbose:
        print "    ", plot

      canvas = ROOT.TCanvas(plotName + "_canvas", plotName, 800, 800)
      plot.Draw();
      canvas.SaveAs("test.png")
      break
    break
