// This file is part of KWIVER, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/kwiver/blob/master/LICENSE for details.

/**
 * \file
 * \brief Interface for serialize_metadata \link
 * kwiver::vital::algo::algorithm_def algorithm
 *        definition \endlink.
 */

#ifndef VITAL_ALGO_SERIALIZE_METADATA_MAP_H_
#define VITAL_ALGO_SERIALIZE_METADATA_MAP_H_

#include <vital/algo/algorithm.h>

#include <vital/algorithm_capabilities.h>

#include <vital/types/metadata.h>
#include <vital/types/metadata_map.h>

#include <vital/vital_config.h>

#include <string>

namespace kwiver {

namespace vital {

namespace algo {

/// An abstract base class for reading and writing metadata maps.
///
/// This class represents an abstract interface for reading and writing video
/// metadata.
class VITAL_ALGO_EXPORT serialize_metadata_map
  : public kwiver::vital::algorithm_def< serialize_metadata_map >
{
public:
  virtual ~serialize_metadata_map() = default;

  /// Return the name of this algorithm
  static std::string static_type_name() { return "serialize_metadata_map"; }

  /// Load metadata from the file.
  ///
  /// \throws kwiver::vital::path_not_exists
  ///   Thrown when the given path does not exist.
  /// \throws kwiver::vital::path_not_a_file
  ///   Thrown when the given path does not point to a file (i.e. it points to
  ///   a directory).
  ///
  /// \param filename The path to the file to load.
  /// \returns A metadata_map_sptr pointing to the data.
  kwiver::vital::metadata_map_sptr load( std::string const& filename ) const;

  /// Load metadata from the file.
  ///
  /// \param fin The stream to read from.
  /// \param filename The path to the file to load.
  /// \returns A metadata_map_sptr pointing to the data.
  kwiver::vital::metadata_map_sptr load(
    std::istream& fin, std::string const& filename = "stream" ) const;

  /// Save metadata to a file.
  ///
  /// \throws kwiver::vital::path_not_exists
  ///   Thrown when the expected containing directory of the given path does
  ///   not exist.
  /// \throws kwiver::vital::path_not_a_directory
  ///   Thrown when the expected containing directory of the given path is not
  ///   actually a directory.
  ///
  /// \param filename The path to the file to save.
  /// \param data Pointer to the metadata to write.
  void save( std::string const& filename,
             kwiver::vital::metadata_map_sptr data ) const;

  /// Save metadata to a stream.
  ///
  /// \param fout The output stream.
  /// \param data Pointer to the metadata to write.
  /// \param filename Filename that generated the stream. Simply for logging.
  void save( std::ostream& fout, kwiver::vital::metadata_map_sptr data,
             std::string const& filename = "stream" ) const;

  /// Return capabilities of concrete implementation.
  ///
  /// This method returns the capabilities for the current metadata
  /// reader/writer.
  ///
  /// \return Reference to supported algorithm capabilities.
  algorithm_capabilities const& get_implementation_capabilities() const;

  void set_configuration( vital::config_block_sptr config ) override;

  bool check_configuration( vital::config_block_sptr config ) const override;

protected:
  serialize_metadata_map();

  void set_capability( algorithm_capabilities::capability_name_t const& name,
                       bool val );

private:
  /// Implementation specific load functionality.
  ///
  /// Concrete implementations of serialize_metadata_map class must provide an
  /// implementation for this method.
  ///
  /// \param filename The path to the file to load.
  /// \returns A metadata_map_sptr pointing to the loaded metadata.
  virtual kwiver::vital::metadata_map_sptr load_(
    std::istream& in_stream, std::string const& filename ) const = 0;

  /// Implementation specific save functionality.
  ///
  /// Concrete implementations of serialize_metadata_map class must provide an
  /// implementation for this method.
  ///
  /// \param filename The path to the file to save.
  /// \param data The metadata_map_sptr pointing to the metadata.
  virtual void save_( std::ostream& out_stream,
                      kwiver::vital::metadata_map_sptr data,
                      std::string const& filename ) const = 0;

  algorithm_capabilities m_capabilities;
};

/// Shared pointer type for generic serialize_metadata_map definition type.
using serialize_metadata_map_sptr = std::shared_ptr< serialize_metadata_map >;

} // namespace algo

} // namespace vital

} // namespace kwiver

#endif
