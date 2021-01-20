//************************************************************************
//  ExaMiniMD v. 1.0
//  Copyright (2018) National Technology & Engineering Solutions of Sandia,
//  LLC (NTESS).
//
//  Under the terms of Contract DE-NA-0003525 with NTESS, the U.S. Government
//  retains certain rights in this software.
//
//  ExaMiniMD is licensed under 3-clause BSD terms of use: Redistribution and
//  use in source and binary forms, with or without modification, are
//  permitted provided that the following conditions are met:
//
//    1. Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//
//    3. Neither the name of the Corporation nor the names of the contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY NTESS "AS IS" AND ANY EXPRESS OR IMPLIED
//  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL NTESS OR THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
//  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
//  Questions? Contact Christian R. Trott (crtrott@sandia.gov)
//************************************************************************

#include<examinimd.h>

// ExaMiniMD can be used as a library
// This main file is simply a driver

#ifdef EXAMINIMD_ENABLE_MPI
#include "mpi.h"
#endif

void hacky_thing_kokkos_should_do_for_me(){
   auto events = Kokkos::Tools::Experimental::get_callbacks();
   auto push = events.push_region;
   auto pop = events.pop_region;
   auto metadata = events.declare_metadata;
   Kokkos::Tools::Experimental::pause_tools();
   Kokkos::Tools::Experimental::set_push_region_callback(push);
   Kokkos::Tools::Experimental::set_pop_region_callback(pop);
   Kokkos::Tools::Experimental::set_declare_metadata_callback(metadata);
}

int main(int argc, char* argv[]) {

   #ifdef EXAMINIMD_ENABLE_MPI
   MPI_Init(&argc,&argv);
   #endif

   Kokkos::initialize(argc,argv);
   hacky_thing_kokkos_should_do_for_me();

   Kokkos::Tools::pushRegion("top_level_timer");

   ExaMiniMD examinimd;
   examinimd.init(argc,argv);
  
   examinimd.run(examinimd.input->nsteps);
   //Kokkos::Tools::declareMetadata("examinimd.nsteps", std::to_string(examinimd.input->nsteps));
   //   examinimd.check_correctness();

   examinimd.print_performance();

   examinimd.shutdown();

   Kokkos::Tools::popRegion();
   Kokkos::finalize();

   #ifdef EXAMINIMD_ENABLE_MPI
   MPI_Finalize();
   #endif
}

