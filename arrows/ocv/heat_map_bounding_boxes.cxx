/*ckwg +29
 * Copyright 2017 by Kitware, Inc.
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

/**
 * \file
 * \brief Implementation of ocv::heat_map_bounding_boxes
 */

#include "heat_map_bounding_boxes.h"

#include <vital/exceptions.h>
#include <vital/types/detected_object.h>
#include <vital/types/detected_object_type.h>

#include <arrows/ocv/image_container.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/eigen.hpp>

namespace kwiver {
namespace arrows {
namespace ocv {

using namespace kwiver::vital;


// ---------------------------------------------------------------------------
// ------------------------------ Sprokit ------------------------------------


/// Private implementation class
class heat_map_bounding_boxes::priv
{
public:  
  /// Constructor
  priv()
  {
  }
};


/// Constructor
heat_map_bounding_boxes
::heat_map_bounding_boxes()
: d_(new priv)
{
  attach_logger( "arrows.ocv.heat_map_bounding_boxes" );
}


/// Destructor
heat_map_bounding_boxes
::~heat_map_bounding_boxes() VITAL_NOTHROW
{
}


/// Get this alg's \link vital::config_block configuration block \endlink
vital::config_block_sptr
heat_map_bounding_boxes
::get_configuration() const
{
  // get base config from base class
  vital::config_block_sptr config = algorithm::get_configuration();

  return config;
}


/// Set this algo's properties via a config block
void
heat_map_bounding_boxes
::set_configuration(vital::config_block_sptr in_config)
{
  // Starting with our generated config_block to ensure that assumed values are present
  // An alternative is to check for key presence before performing a get_value() call.
  vital::config_block_sptr config = this->get_configuration();
  config->merge_config(in_config);
}


bool
heat_map_bounding_boxes
::check_configuration(vital::config_block_sptr config) const
{
  return true;
}


/// Return homography to stabilize the image_src relative to the key frame 
detected_object_set_sptr
heat_map_bounding_boxes
::detect(image_container_sptr image_data) const
{
  if ( !image_data)
  {
    throw vital::invalid_data("Inputs to ocv::heat_map_bounding_boxes are null");
  }

  cv::Mat cv_src = ocv::image_container::vital_to_ocv(image_data->get_image());
  auto detected_objects = std::make_shared< detected_object_set >();
  
  std::vector< std::vector<cv::Point> > contours, filtered_contours;
  cv::findContours(cv_src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, 
                   cv::Point(0, 0));
  
  double conf = 1.0;
  double prob = 1.0;
  auto dot = std::make_shared< detected_object_type >();
  const std::string class_name("mover");
  dot->set_score( class_name, prob );
  
  for(unsigned j=0; j < contours.size(); ++j)
  {
    //std::cout << "Contour " << j << ": " << contourArea(contours[j], false) << std::endl;
    if( contourArea(contours[j], false) >= 30 )
    {
      cv::Rect cv_bbox = cv::boundingRect(contours[j]);
      kwiver::vital::bounding_box_d bbox( cv_bbox.x, cv_bbox.y, 
                                          cv_bbox.x + cv_bbox.width, 
                                          cv_bbox.y + cv_bbox.height );
      
      detected_objects->add(
        std::make_shared< kwiver::vital::detected_object >( bbox, conf, dot ) );
    }
  }
  
  return detected_objects;
}


} // end namespace ocv
} // end namespace arrows
} // end namespace kwiver
