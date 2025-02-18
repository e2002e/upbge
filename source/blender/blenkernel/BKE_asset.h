/* SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup bke
 */

#pragma once

#include "BLI_compiler_attrs.h"
#include "BLI_utildefines.h"

#include "DNA_asset_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct AssetLibraryReference;
struct AssetMetaData;
struct BlendDataReader;
struct BlendWriter;
struct ID;
struct IDProperty;
struct PreviewImage;

/** C handle for #bke::AssetRepresentation. */
typedef struct AssetRepresentation AssetRepresentation;

typedef void (*PreSaveFn)(void *asset_ptr, struct AssetMetaData *asset_data);

typedef struct AssetTypeInfo {
  /**
   * For local assets (assets in the current .blend file), a callback to execute before the file is
   * saved.
   */
  PreSaveFn pre_save_fn;
} AssetTypeInfo;

struct AssetMetaData *BKE_asset_metadata_create(void);
void BKE_asset_metadata_free(struct AssetMetaData **asset_data);

struct AssetTagEnsureResult {
  struct AssetTag *tag;
  /* Set to false if a tag of this name was already present. */
  bool is_new;
};

struct AssetTag *BKE_asset_metadata_tag_add(struct AssetMetaData *asset_data, const char *name);
/**
 * Make sure there is a tag with name \a name, create one if needed.
 */
struct AssetTagEnsureResult BKE_asset_metadata_tag_ensure(struct AssetMetaData *asset_data,
                                                          const char *name);
void BKE_asset_metadata_tag_remove(struct AssetMetaData *asset_data, struct AssetTag *tag);

/** Clean up the catalog ID (white-spaces removed, length reduced, etc.) and assign it. */
void BKE_asset_metadata_catalog_id_clear(struct AssetMetaData *asset_data);
void BKE_asset_metadata_catalog_id_set(struct AssetMetaData *asset_data,
                                       bUUID catalog_id,
                                       const char *catalog_simple_name);

void BKE_asset_library_reference_init_default(struct AssetLibraryReference *library_ref);

void BKE_asset_metadata_idprop_ensure(struct AssetMetaData *asset_data, struct IDProperty *prop);
struct IDProperty *BKE_asset_metadata_idprop_find(const struct AssetMetaData *asset_data,
                                                  const char *name) ATTR_WARN_UNUSED_RESULT;

struct PreviewImage *BKE_asset_metadata_preview_get_from_id(const struct AssetMetaData *asset_data,
                                                            const struct ID *owner_id);

void BKE_asset_metadata_write(struct BlendWriter *writer, struct AssetMetaData *asset_data);
void BKE_asset_metadata_read(struct BlendDataReader *reader, struct AssetMetaData *asset_data);

const char *BKE_asset_representation_name_get(const AssetRepresentation *asset)
    ATTR_WARN_UNUSED_RESULT;
AssetMetaData *BKE_asset_representation_metadata_get(const AssetRepresentation *asset)
    ATTR_WARN_UNUSED_RESULT;
bool BKE_asset_representation_is_local_id(const AssetRepresentation *asset)
    ATTR_WARN_UNUSED_RESULT;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#  include <memory>

[[nodiscard]] std::unique_ptr<AssetMetaData> BKE_asset_metadata_move_to_unique_ptr(
    AssetMetaData *asset_data);

#endif
