/* SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup bke
 */

#pragma once

struct IDRemapper;
struct Main;

#ifdef __cplusplus
extern "C" {
#endif

/** Forward declaration, defined in intern/asset_library.hh */
typedef struct AssetLibrary AssetLibrary;

/**
 * Return the #AssetLibrary rooted at the given directory path.
 *
 * Will return the same pointer for repeated calls, until another blend file is loaded.
 *
 * To get the in-memory-only "current file" asset library, pass an empty path.
 */
struct AssetLibrary *BKE_asset_library_load(const char *library_path);

/** Look up the asset's catalog and copy its simple name into #asset_data. */
void BKE_asset_library_refresh_catalog_simplename(struct AssetLibrary *asset_library,
                                                  struct AssetMetaData *asset_data);

/** Return whether any loaded AssetLibrary has unsaved changes to its catalogs. */
bool BKE_asset_library_has_any_unsaved_catalogs(void);

/** An asset library can include local IDs (IDs in the current file). Their pointers need to be
 * remapped on change (or assets removed as IDs gets removed). */
void BKE_asset_library_remap_ids(struct IDRemapper *mappings);

#ifdef __cplusplus
}
#endif
