import os,subprocess,ROOT
import pickle
import re
import time
import shutil

if __name__ == "__main__":
  import argparse
  import os
  import sys

  parser = argparse.ArgumentParser(description='Process the command line options')
  parser.add_argument('-d', '--dryRun', action='store_true', help='Do a dry run (i.e. do not actually run the potentially dangerous commands but print them to the screen)')
  parser.add_argument('-v', '--verbose', action='store_true', help='Whether to print verbose output')
  #parser.add_argument('-o', '--outDirectory', required=True, help='Name of the output directory')
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
      "name": "h_Arm1xi",
      "logy": False,
      "logz": False,
    },
    "Arm2Xi": {
      "name": "h_Arm2xi",
      "logy": False,
      "logz": False,
    },
  }

  file = ROOT.TFile.Open(args.inputFile, "READ")

  for condition in toPlot:
    plotDirectory = file.Get(toPlot[condition])
    for plotName in plots:
      plot = plotDirectory.Get(plots[plotName]["name"])

      canvas = ROOT.TCanvas(800,800)
      plot->Draw();
      canvas.SaveAs("test.png")
      break
    break
