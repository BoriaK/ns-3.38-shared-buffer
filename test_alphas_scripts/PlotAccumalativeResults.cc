/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/gnuplot.h"


using namespace ns3;


// std::string
// ToString (uint32_t value)
// {
//   std::stringstream ss;
//   ss << value;
//   return ss.str();
// }


void
CreateSingle2DPlotFile(std::string usedAlgorythm, std::string implementation)  // for a single plot with N data-sets
{
  std::string gnuPlotFileLocation = "./test_alphas_scripts/";

  // // Set up some default values for the simulation.
  std::string trace_parameter = usedAlgorythm + "_TestAccumulativeStatistics";
  std::string gnuPlotFileName = gnuPlotFileLocation + trace_parameter + "_" + implementation + ".plt";

  // command line needs to be in ./ns-3-dev-git$ inorder for the script to produce gnuplot correctly///
  system (("gnuplot " + gnuPlotFileName).c_str ());

}

int main (int argc, char *argv[])
{ 
  std::string usedAlgorythm = "All";  // "DT"/"FB"/"All"
  std::string implementation = "MQueue_5ToS";  // "FIFO"/"MQueue_2ToS"/"MQueue_5ToS"
  CreateSingle2DPlotFile(usedAlgorythm, implementation);
}