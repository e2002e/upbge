/* SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup bke
 */

#include <stdexcept>

#include "DNA_ID.h"
#include "DNA_asset_types.h"

#include "BKE_asset.h"
#include "BKE_asset_representation.hh"

namespace blender::bke {

AssetRepresentation::AssetRepresentation(StringRef name, std::unique_ptr<AssetMetaData> metadata)
    : is_local_id_(false), external_asset_()
{
  external_asset_.name = name;
  external_asset_.metadata_ = std::move(metadata);
}

AssetRepresentation::AssetRepresentation(ID &id) : is_local_id_(true), local_asset_id_(&id)
{
  if (!id.asset_data) {
    throw std::invalid_argument("Passed ID is not an asset");
  }
}

AssetRepresentation::AssetRepresentation(AssetRepresentation &&other)
    : is_local_id_(other.is_local_id_)
{
  if (is_local_id_) {
    local_asset_id_ = other.local_asset_id_;
    other.local_asset_id_ = nullptr;
  }
  else {
    external_asset_ = std::move(other.external_asset_);
  }
}

AssetRepresentation::~AssetRepresentation()
{
  if (!is_local_id_) {
    external_asset_.~ExternalAsset();
  }
}

StringRefNull AssetRepresentation::get_name() const
{
  if (is_local_id_) {
    return local_asset_id_->name + 2;
  }

  return external_asset_.name;
}

AssetMetaData &AssetRepresentation::get_metadata() const
{
  return is_local_id_ ? *local_asset_id_->asset_data : *external_asset_.metadata_;
}

bool AssetRepresentation::is_local_id() const
{
  return is_local_id_;
}

}  // namespace blender::bke

/* ---------------------------------------------------------------------- */
/** \name C-API
 * \{ */

using namespace blender;

const char *BKE_asset_representation_name_get(const AssetRepresentation *asset_handle)
{
  const bke::AssetRepresentation *asset = reinterpret_cast<const bke::AssetRepresentation *>(
      asset_handle);
  return asset->get_name().c_str();
}

AssetMetaData *BKE_asset_representation_metadata_get(const AssetRepresentation *asset_handle)
{
  const bke::AssetRepresentation *asset = reinterpret_cast<const bke::AssetRepresentation *>(
      asset_handle);
  return &asset->get_metadata();
}

bool BKE_asset_representation_is_local_id(const AssetRepresentation *asset_handle)
{
  const bke::AssetRepresentation *asset = reinterpret_cast<const bke::AssetRepresentation *>(
      asset_handle);
  return asset->is_local_id();
}

/** \} */
