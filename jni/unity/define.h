/*
 * define.h
 *
 *  Created on: 2019-1-25
 *      Author: Administrator
 */

#ifndef UNITY_DEFINE_H_
#define UNITY_DEFINE_H_

/* Basic data types */
typedef int            gboolean;
typedef int            gint;
typedef unsigned int   guint;
typedef short          gshort;
typedef unsigned short gushort;
typedef long           glong;
typedef unsigned long  gulong;
typedef void *         gpointer;
typedef const void *   gconstpointer;
typedef char           gchar;
typedef unsigned char  guchar;
/* Types defined in terms of the stdint.h */
typedef int8_t         gint8;
typedef uint8_t        guint8;
typedef int16_t        gint16;
typedef uint16_t       guint16;
typedef int32_t        gint32;
typedef uint32_t       guint32;
typedef int64_t        gint64;
typedef uint64_t       guint64;
typedef float          gfloat;
typedef double         gdouble;
typedef uint16_t       gunichar2;

typedef enum {
	MONO_IMAGE_OK,
	MONO_IMAGE_ERROR_ERRNO,
	MONO_IMAGE_MISSING_ASSEMBLYREF,
	MONO_IMAGE_IMAGE_INVALID
} MonoImageOpenStatus;

struct MonoImage {
	/*
	 * The number of assemblies which reference this MonoImage though their 'image'
	 * field plus the number of images which reference this MonoImage through their
	 * 'modules' field, plus the number of threads holding temporary references to
	 * this image between calls of mono_image_open () and mono_image_close ().
	 */
	int   ref_count;
	void *raw_data_handle;
	char *raw_data;
	guint32 raw_data_len;
	guint8 raw_buffer_used    : 1;
	guint8 raw_data_allocated : 1;

#ifdef USE_COREE
	/* Module was loaded using LoadLibrary. */
	guint8 is_module_handle : 1;

	/* Module entry point is _CorDllMain. */
	guint8 has_entry_point : 1;
#endif

	/* Whenever this is a dynamically emitted module */
	guint8 dynamic : 1;

	/* Whenever this is a reflection only image */
	guint8 ref_only : 1;

	/* Whenever this image contains uncompressed metadata */
	guint8 uncompressed_metadata : 1;

	guint8 checked_module_cctor : 1;
	guint8 has_module_cctor : 1;

	guint8 idx_string_wide : 1;
	guint8 idx_guid_wide : 1;
	guint8 idx_blob_wide : 1;

	/* Whenever this image is considered as platform code for the CoreCLR security model */
	guint8 core_clr_platform_code : 1;

	char *name;
};

#endif /* DEFINE_H_ */
