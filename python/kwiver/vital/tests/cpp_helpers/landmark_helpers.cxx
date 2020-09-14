/*ckwg +29
 * Copyright 2020 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <vital/types/landmark.h>

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vital/util/demangle.h>
#include <memory>
namespace py = pybind11;
namespace kv = kwiver::vital;

// Helpers to call pure virtual functions from base reference.
// We'll use these to test that these methods can be overriden in C++
PYBIND11_MODULE( landmark_helpers, m )
{
  m.def( "call_clone", [] ( const kv::landmark& self)
  {
    return self.clone();
  });
  m.def( "call_get_type", [] ( const kv::landmark& self )
  {
    return kv::demangle( self.data_type().name() );
  });

  m.def( "call_loc", [] ( const kv::landmark& self )
  {
    return self.loc();
  });

  m.def( "call_scale", [] ( const kv::landmark& self )
  {
    return self.scale();
  });

  m.def( "call_normal", [] ( const kv::landmark& self )
  {
    return self.normal();
  });

  m.def( "call_covar", [] ( kv::landmark& self )
  {
    return self.covar();
  });

  m.def( "call_observations", [] ( kv::landmark& self )
  {
    return self.observations();
  });

  m.def( "call_cos_obs_angle", [] ( kv::landmark& self )
  {
    return self.cos_obs_angle();
  });
}
