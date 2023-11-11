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
#include <vector>
#include <utility>
#include <string>
#include <list>
#include <array>
#include <filesystem>

#include "sharedBuffer_via_MultiQueues_2ToS.cc"


int main ()
{ 
  std::string traffic_control_type = "SharedBuffer_DT_v01"; // "SharedBuffer_DT_v01"/"SharedBuffer_FB_v01"
  bool accumulateStats = false; // true/false

  // option 1: run a specific alpha high/low value:
  double_t alpha_high = 10;
  double_t alpha_low = 10;

  // viaMQueues5ToS(traffic_control_type, alpha_high, alpha_low, accumulateStats);
  viaMQueues2ToS(traffic_control_type, alpha_high, alpha_low, accumulateStats);



  // option 2: run over a specific array of alphas high/low:
  // std::array<double_t, 21> alpha_high_array = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0.5};
  // std::array<double_t, 21> alpha_low_array = {0.5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  // std::array<double_t, 2> alpha_high_array = {20, 19};
  // std::array<double_t, 2> alpha_low_array = {0.5, 1};
  
  // for (size_t i = 0; i < alpha_high_array.size(); i++)
  // {
  //   viaMQueues5ToS(traffic_control_type, alpha_high_array[i], alpha_low_array[i], accumulateStats);
  // }


  // option 3: run over all alphas high/low in range:
  // for (size_t a_h = 20; a_h > 0; a_h--)
  // {
  //   for (size_t a_l = 1; a_l < 21; a_l++)
  //   {
  //     viaMQueues5ToS(traffic_control_type, implementation, a_h, a_l, accumulateStats);
  //   }
  // }

  return 0;
}