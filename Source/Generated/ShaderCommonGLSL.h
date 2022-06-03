// This file was generated by GenerateShaderCommon.py

#define MAX_STATIC_VERTEX_COUNT (1048576)
#define MAX_DYNAMIC_VERTEX_COUNT (2097152)
#define MAX_INDEXED_PRIMITIVE_COUNT (1048576)
#define MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT (4096)
#define MAX_BOTTOM_LEVEL_GEOMETRIES_COUNT_POW (12)
#define MAX_GEOMETRY_PRIMITIVE_COUNT (1048576)
#define MAX_GEOMETRY_PRIMITIVE_COUNT_POW (20)
#define LOWER_BOTTOM_LEVEL_GEOMETRIES_COUNT (256)
#define MAX_TOP_LEVEL_INSTANCE_COUNT (45)
#define BINDING_VERTEX_BUFFER_STATIC (0)
#define BINDING_VERTEX_BUFFER_DYNAMIC (1)
#define BINDING_INDEX_BUFFER_STATIC (2)
#define BINDING_INDEX_BUFFER_DYNAMIC (3)
#define BINDING_GEOMETRY_INSTANCES (4)
#define BINDING_GEOMETRY_INSTANCES_MATCH_PREV (5)
#define BINDING_PREV_POSITIONS_BUFFER_DYNAMIC (6)
#define BINDING_PREV_INDEX_BUFFER_DYNAMIC (7)
#define BINDING_PER_TRIANGLE_INFO (8)
#define BINDING_GLOBAL_UNIFORM (0)
#define BINDING_ACCELERATION_STRUCTURE_MAIN (0)
#define BINDING_TEXTURES (0)
#define BINDING_CUBEMAPS (0)
#define BINDING_RENDER_CUBEMAP (0)
#define BINDING_BLUE_NOISE (0)
#define BINDING_LUM_HISTOGRAM (0)
#define BINDING_LIGHT_SOURCES (0)
#define BINDING_LIGHT_SOURCES_PREV (1)
#define BINDING_LIGHT_SOURCES_MATCH_PREV (2)
#define BINDING_PLAIN_LIGHT_LIST (3)
#define BINDING_SECTOR_TO_LIGHT_LIST_REGION (4)
#define BINDING_LENS_FLARES_CULLING_INPUT (0)
#define BINDING_LENS_FLARES_DRAW_CMDS (1)
#define BINDING_DRAW_LENS_FLARES_INSTANCES (0)
#define BINDING_DECAL_INSTANCES (0)
#define INSTANCE_CUSTOM_INDEX_FLAG_DYNAMIC (1 << 0)
#define INSTANCE_CUSTOM_INDEX_FLAG_FIRST_PERSON (1 << 1)
#define INSTANCE_CUSTOM_INDEX_FLAG_FIRST_PERSON_VIEWER (1 << 2)
#define INSTANCE_CUSTOM_INDEX_FLAG_REFLECT_REFRACT (1 << 3)
#define INSTANCE_CUSTOM_INDEX_FLAG_SKY (1 << 4)
#define INSTANCE_MASK_WORLD_0 (1)
#define INSTANCE_MASK_WORLD_1 (2)
#define INSTANCE_MASK_WORLD_2 (4)
#define INSTANCE_MASK_RESERVED_0 (8)
#define INSTANCE_MASK_RESERVED_1 (16)
#define INSTANCE_MASK_REFLECT_REFRACT (32)
#define INSTANCE_MASK_FIRST_PERSON (64)
#define INSTANCE_MASK_FIRST_PERSON_VIEWER (128)
#define PAYLOAD_INDEX_DEFAULT (0)
#define PAYLOAD_INDEX_SHADOW (1)
#define SBT_INDEX_RAYGEN_PRIMARY (0)
#define SBT_INDEX_RAYGEN_REFL_REFR (1)
#define SBT_INDEX_RAYGEN_DIRECT (2)
#define SBT_INDEX_RAYGEN_INDIRECT (3)
#define SBT_INDEX_MISS_DEFAULT (0)
#define SBT_INDEX_MISS_SHADOW (1)
#define SBT_INDEX_HITGROUP_FULLY_OPAQUE (0)
#define SBT_INDEX_HITGROUP_ALPHA_TESTED (1)
#define MATERIAL_ALBEDO_ALPHA_INDEX (0)
#define MATERIAL_ROUGHNESS_METALLIC_EMISSION_INDEX (1)
#define MATERIAL_NORMAL_INDEX (2)
#define MATERIAL_NO_TEXTURE (0)
#define MATERIAL_BLENDING_FLAG_OPAQUE (1 << 0)
#define MATERIAL_BLENDING_FLAG_ALPHA (1 << 1)
#define MATERIAL_BLENDING_FLAG_ADD (1 << 2)
#define MATERIAL_BLENDING_FLAG_SHADE (1 << 3)
#define MATERIAL_BLENDING_FLAG_BIT_COUNT (4)
#define MATERIAL_BLENDING_MASK_FIRST_LAYER (15)
#define MATERIAL_BLENDING_MASK_SECOND_LAYER (240)
#define MATERIAL_BLENDING_MASK_THIRD_LAYER (3840)
#define GEOM_INST_FLAG_RESERVED_0 (1 << 13)
#define GEOM_INST_FLAG_RESERVED_1 (1 << 14)
#define GEOM_INST_FLAG_RESERVED_2 (1 << 15)
#define GEOM_INST_FLAG_RESERVED_3 (1 << 16)
#define GEOM_INST_FLAG_RESERVED_4 (1 << 17)
#define GEOM_INST_FLAG_RESERVED_5 (1 << 18)
#define GEOM_INST_FLAG_RESERVED_6 (1 << 19)
#define GEOM_INST_FLAG_IGNORE_REFL_REFR_AFTER (1 << 20)
#define GEOM_INST_FLAG_REFL_REFR_ALBEDO_MULT (1 << 21)
#define GEOM_INST_FLAG_REFL_REFR_ALBEDO_ADD (1 << 22)
#define GEOM_INST_FLAG_NO_MEDIA_CHANGE (1 << 23)
#define GEOM_INST_FLAG_REFRACT (1 << 24)
#define GEOM_INST_FLAG_REFLECT (1 << 25)
#define GEOM_INST_FLAG_PORTAL (1 << 26)
#define GEOM_INST_FLAG_MEDIA_TYPE_WATER (1 << 27)
#define GEOM_INST_FLAG_MEDIA_TYPE_GLASS (1 << 28)
#define GEOM_INST_FLAG_GENERATE_NORMALS (1 << 29)
#define GEOM_INST_FLAG_INVERTED_NORMALS (1 << 30)
#define GEOM_INST_FLAG_IS_MOVABLE (1 << 31)
#define SKY_TYPE_COLOR (0)
#define SKY_TYPE_CUBEMAP (1)
#define SKY_TYPE_RASTERIZED_GEOMETRY (2)
#define BLUE_NOISE_TEXTURE_COUNT (128)
#define BLUE_NOISE_TEXTURE_SIZE (128)
#define BLUE_NOISE_TEXTURE_SIZE_POW (7)
#define COMPUTE_COMPOSE_GROUP_SIZE_X (16)
#define COMPUTE_COMPOSE_GROUP_SIZE_Y (16)
#define COMPUTE_BLOOM_UPSAMPLE_GROUP_SIZE_X (16)
#define COMPUTE_BLOOM_UPSAMPLE_GROUP_SIZE_Y (16)
#define COMPUTE_BLOOM_DOWNSAMPLE_GROUP_SIZE_X (16)
#define COMPUTE_BLOOM_DOWNSAMPLE_GROUP_SIZE_Y (16)
#define COMPUTE_BLOOM_APPLY_GROUP_SIZE_X (16)
#define COMPUTE_BLOOM_APPLY_GROUP_SIZE_Y (16)
#define COMPUTE_BLOOM_STEP_COUNT (5)
#define COMPUTE_EFFECT_GROUP_SIZE_X (16)
#define COMPUTE_EFFECT_GROUP_SIZE_Y (16)
#define COMPUTE_LUM_HISTOGRAM_GROUP_SIZE_X (16)
#define COMPUTE_LUM_HISTOGRAM_GROUP_SIZE_Y (16)
#define COMPUTE_LUM_HISTOGRAM_BIN_COUNT (256)
#define COMPUTE_VERT_PREPROC_GROUP_SIZE_X (256)
#define VERT_PREPROC_MODE_ONLY_DYNAMIC (0)
#define VERT_PREPROC_MODE_DYNAMIC_AND_MOVABLE (1)
#define VERT_PREPROC_MODE_ALL (2)
#define GRADIENT_ESTIMATION_ENABLED (1)
#define COMPUTE_GRADIENT_SAMPLES_GROUP_SIZE_X (16)
#define COMPUTE_GRADIENT_MERGING_GROUP_SIZE_X (16)
#define COMPUTE_GRADIENT_ATROUS_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_TEMPORAL_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_VARIANCE_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_ATROUS_GROUP_SIZE_X (16)
#define COMPUTE_SVGF_ATROUS_ITERATION_COUNT (4)
#define COMPUTE_ASVGF_STRATA_SIZE (3)
#define COMPUTE_ASVGF_GRADIENT_ATROUS_ITERATION_COUNT (4)
#define COMPUTE_INDIRECT_DRAW_FLARES_GROUP_SIZE_X (256)
#define LENS_FLARES_MAX_DRAW_CMD_COUNT (512)
#define DEBUG_SHOW_FLAG_MOTION_VECTORS (1 << 0)
#define DEBUG_SHOW_FLAG_GRADIENTS (1 << 1)
#define DEBUG_SHOW_FLAG_SECTORS (1 << 2)
#define DEBUG_SHOW_FLAG_UNFILTERED_DIFF (1 << 3)
#define DEBUG_SHOW_FLAG_UNFILTERED_SPEC (1 << 4)
#define DEBUG_SHOW_FLAG_UNFILTERED_INDIR (1 << 5)
#define MAX_RAY_LENGTH (10000.0)
#define MEDIA_TYPE_VACUUM (0)
#define MEDIA_TYPE_WATER (1)
#define MEDIA_TYPE_GLASS (2)
#define MEDIA_TYPE_COUNT (3)
#define GEOM_INST_NO_TRIANGLE_INFO (UINT32_MAX)
#define SECTOR_INDEX_NONE (32767)
#define LIGHT_TYPE_NONE (0)
#define LIGHT_TYPE_DIRECTIONAL (1)
#define LIGHT_TYPE_SPHERE (2)
#define LIGHT_TYPE_TRIANGLE (3)
#define LIGHT_TYPE_SPOT (4)

#define FIDELITY_SUPER_RESOLUTION_GAMMA_SPACE (3.0)
#define SURFACE_POSITION_INCORRECT (10000000.0)

struct ShVertexBufferStatic
{
    float positions[3145728];
    float normals[3145728];
    float texCoords[2097152];
    float texCoordsLayer1[2097152];
    float texCoordsLayer2[2097152];
};

struct ShVertexBufferDynamic
{
    float positions[6291456];
    float normals[6291456];
    float texCoords[4194304];
};

struct ShGlobalUniform
{
    mat4 view;
    mat4 invView;
    mat4 viewPrev;
    mat4 projection;
    mat4 invProjection;
    mat4 projectionPrev;
    uint positionsStride;
    uint normalsStride;
    uint texCoordsStride;
    float renderWidth;
    float renderHeight;
    uint frameId;
    float timeDelta;
    float minLogLuminance;
    float maxLogLuminance;
    float luminanceWhitePoint;
    uint stopEyeAdaptation;
    uint lightCountDirectional;
    float polyLightSpotlightFactor;
    uint skyType;
    float skyColorMultiplier;
    uint skyCubemapIndex;
    vec4 skyColorDefault;
    vec4 skyViewerPosition;
    vec4 cameraPosition;
    uint debugShowFlags;
    float firefliesClamp;
    uint lightCount;
    uint lightCountPrev;
    float emissionMapBoost;
    float emissionMaxScreenColor;
    float normalMapStrength;
    float skyColorSaturation;
    float _unused0;
    float _unused1;
    float _unused2;
    float bloomEmissionSaturationBias;
    uint maxBounceShadowsDirectionalLights;
    uint maxBounceShadowsLights;
    uint rayCullBackFaces;
    uint rayCullMaskWorld;
    float bloomThreshold;
    float bloomThresholdLength;
    float bloomUpsampleRadius;
    float bloomIntensity;
    float bloomEmissionMultiplier;
    float bloomSkyMultiplier;
    float rayLength;
    uint reflectRefractMaxDepth;
    uint cameraMediaType;
    float indexOfRefractionWater;
    float indexOfRefractionGlass;
    float waterTextureDerivativesMultiplier;
    uint enableShadowsFromReflRefr;
    uint enableIndirectFromReflRefr;
    uint forceNoWaterRefraction;
    uint waterNormalTextureIndex;
    uint noBackfaceReflForNoMediaChange;
    float time;
    float waterWaveSpeed;
    float waterWaveStrength;
    vec4 waterExtinction;
    vec4 portalInputToOutputTransform0;
    vec4 portalInputToOutputTransform1;
    vec4 portalInputToOutputTransform2;
    vec4 portalInputPosition;
    float cameraRayConeSpreadAngle;
    float waterTextureAreaScale;
    uint useSqrtRoughnessForIndirect;
    float upscaledRenderWidth;
    vec4 worldUpVector;
    float upscaledRenderHeight;
    float jitterX;
    float jitterY;
    float primaryRayMinDist;
    vec4 directionalLight_color;
    vec4 directionalLight_data_0;
    uint rayCullMaskWorld_Shadow;
    uint lensFlareCullingInputCount;
    uint applyViewProjToLensFlares;
    uint areFramebufsInitedByRT;
    ivec4 instanceGeomInfoOffset[12];
    ivec4 instanceGeomInfoOffsetPrev[12];
    ivec4 instanceGeomCount[12];
    mat4 viewProjCubemap[6];
    mat4 skyCubemapRotationTransform;
};

struct ShGeometryInstance
{
    mat4 model;
    mat4 prevModel;
    vec4 materialColors[3];
    uint materials0A;
    uint materials0B;
    uint materials0C;
    uint materials1A;
    uint materials1B;
    uint materials1C;
    uint materials2A;
    uint materials2B;
    uint sectorArrayIndex;
    uint flags;
    uint baseVertexIndex;
    uint baseIndexIndex;
    uint prevBaseVertexIndex;
    uint prevBaseIndexIndex;
    uint vertexCount;
    uint indexCount;
    float defaultRoughness;
    float defaultMetallicity;
    float defaultEmission;
    uint triangleArrayIndex;
};

struct ShTonemapping
{
    uint histogram[256];
    float avgLuminance;
};

struct ShLightEncoded
{
    vec3 color;
    uint lightType;
    vec4 data_0;
    vec4 data_1;
    vec4 data_2;
};

struct ShVertPreprocessing
{
    uint tlasInstanceCount;
    uint tlasInstanceIsDynamicBits[2];
};

struct ShIndirectDrawCommand
{
    uint indexCount;
    uint instanceCount;
    uint firstIndex;
    int vertexOffset;
    uint firstInstance;
    float positionToCheck_X;
    float positionToCheck_Y;
    float positionToCheck_Z;
};

struct ShLensFlareInstance
{
    uint textureIndex;
};

struct ShDecalInstance
{
    mat4 transform;
    uint textureAlbedoAlpha;
    uint textureRougnessMetallic;
    uint textureNormals;
    uint __pad0;
};

#ifdef DESC_SET_FRAMEBUFFERS

// framebuffer indices
#define FB_IMAGE_INDEX_ALBEDO 0
#define FB_IMAGE_INDEX_NORMAL 1
#define FB_IMAGE_INDEX_NORMAL_PREV 2
#define FB_IMAGE_INDEX_NORMAL_GEOMETRY 3
#define FB_IMAGE_INDEX_NORMAL_GEOMETRY_PREV 4
#define FB_IMAGE_INDEX_METALLIC_ROUGHNESS 5
#define FB_IMAGE_INDEX_METALLIC_ROUGHNESS_PREV 6
#define FB_IMAGE_INDEX_DEPTH 7
#define FB_IMAGE_INDEX_DEPTH_PREV 8
#define FB_IMAGE_INDEX_RANDOM_SEED 9
#define FB_IMAGE_INDEX_RANDOM_SEED_PREV 10
#define FB_IMAGE_INDEX_MOTION 11
#define FB_IMAGE_INDEX_UNFILTERED_DIRECT 12
#define FB_IMAGE_INDEX_UNFILTERED_SPECULAR 13
#define FB_IMAGE_INDEX_UNFILTERED_INDIRECT_S_H_R 14
#define FB_IMAGE_INDEX_UNFILTERED_INDIRECT_S_H_G 15
#define FB_IMAGE_INDEX_UNFILTERED_INDIRECT_S_H_B 16
#define FB_IMAGE_INDEX_SURFACE_POSITION 17
#define FB_IMAGE_INDEX_VISIBILITY_BUFFER 18
#define FB_IMAGE_INDEX_VISIBILITY_BUFFER_PREV 19
#define FB_IMAGE_INDEX_SECTOR_INDEX 20
#define FB_IMAGE_INDEX_SECTOR_INDEX_PREV 21
#define FB_IMAGE_INDEX_VIEW_DIRECTION 22
#define FB_IMAGE_INDEX_PRIMARY_TO_REFL_REFR 23
#define FB_IMAGE_INDEX_THROUGHPUT 24
#define FB_IMAGE_INDEX_PRE_FINAL 25
#define FB_IMAGE_INDEX_FINAL 26
#define FB_IMAGE_INDEX_UPSCALED_PING 27
#define FB_IMAGE_INDEX_UPSCALED_PONG 28
#define FB_IMAGE_INDEX_UPSCALED_HISTORY 29
#define FB_IMAGE_INDEX_UPSCALED_HISTORY_PREV 30
#define FB_IMAGE_INDEX_DEPTH_DLSS 31
#define FB_IMAGE_INDEX_MOTION_DLSS 32
#define FB_IMAGE_INDEX_ACCUM_HISTORY_LENGTH 33
#define FB_IMAGE_INDEX_ACCUM_HISTORY_LENGTH_PREV 34
#define FB_IMAGE_INDEX_DIFF_ACCUM_COLOR 35
#define FB_IMAGE_INDEX_DIFF_ACCUM_COLOR_PREV 36
#define FB_IMAGE_INDEX_DIFF_ACCUM_MOMENTS 37
#define FB_IMAGE_INDEX_DIFF_ACCUM_MOMENTS_PREV 38
#define FB_IMAGE_INDEX_DIFF_COLOR_HISTORY 39
#define FB_IMAGE_INDEX_DIFF_PING_COLOR_AND_VARIANCE 40
#define FB_IMAGE_INDEX_DIFF_PONG_COLOR_AND_VARIANCE 41
#define FB_IMAGE_INDEX_SPEC_ACCUM_COLOR 42
#define FB_IMAGE_INDEX_SPEC_ACCUM_COLOR_PREV 43
#define FB_IMAGE_INDEX_SPEC_PING_COLOR 44
#define FB_IMAGE_INDEX_SPEC_PONG_COLOR 45
#define FB_IMAGE_INDEX_INDIR_ACCUM_S_H_R 46
#define FB_IMAGE_INDEX_INDIR_ACCUM_S_H_R_PREV 47
#define FB_IMAGE_INDEX_INDIR_ACCUM_S_H_G 48
#define FB_IMAGE_INDEX_INDIR_ACCUM_S_H_G_PREV 49
#define FB_IMAGE_INDEX_INDIR_ACCUM_S_H_B 50
#define FB_IMAGE_INDEX_INDIR_ACCUM_S_H_B_PREV 51
#define FB_IMAGE_INDEX_INDIR_PING_S_H_R 52
#define FB_IMAGE_INDEX_INDIR_PING_S_H_G 53
#define FB_IMAGE_INDEX_INDIR_PING_S_H_B 54
#define FB_IMAGE_INDEX_INDIR_PONG_S_H_R 55
#define FB_IMAGE_INDEX_INDIR_PONG_S_H_G 56
#define FB_IMAGE_INDEX_INDIR_PONG_S_H_B 57
#define FB_IMAGE_INDEX_ATROUS_FILTERED_VARIANCE 58
#define FB_IMAGE_INDEX_BLOOM_MIP1 59
#define FB_IMAGE_INDEX_BLOOM_MIP2 60
#define FB_IMAGE_INDEX_BLOOM_MIP3 61
#define FB_IMAGE_INDEX_BLOOM_MIP4 62
#define FB_IMAGE_INDEX_BLOOM_MIP5 63
#define FB_IMAGE_INDEX_BLOOM_RESULT 64
#define FB_IMAGE_INDEX_WIPE_EFFECT_SOURCE 65
#define FB_IMAGE_INDEX_GRADIENT_SAMPLES 66
#define FB_IMAGE_INDEX_GRADIENT_SAMPLES_PREV 67
#define FB_IMAGE_INDEX_DIFF_AND_SPEC_PING_GRADIENT 68
#define FB_IMAGE_INDEX_DIFF_AND_SPEC_PONG_GRADIENT 69
#define FB_IMAGE_INDEX_INDIR_PING_GRADIENT 70
#define FB_IMAGE_INDEX_INDIR_PONG_GRADIENT 71

// framebuffers
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 0, rgba16f) uniform image2D framebufAlbedo;
#endif
layout(set = DESC_SET_FRAMEBUFFERS, binding = 1, r32ui) uniform uimage2D framebufNormal;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 2, r32ui) uniform uimage2D framebufNormal_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 3, r32ui) uniform uimage2D framebufNormalGeometry;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 4, r32ui) uniform uimage2D framebufNormalGeometry_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 5, rgba8) uniform image2D framebufMetallicRoughness;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 6, rgba8) uniform image2D framebufMetallicRoughness_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 7, rgba32f) uniform image2D framebufDepth;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 8, rgba32f) uniform image2D framebufDepth_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 9, r32ui) uniform uimage2D framebufRandomSeed;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 10, r32ui) uniform uimage2D framebufRandomSeed_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 11, rgba16f) uniform image2D framebufMotion;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 12, r32ui) uniform uimage2D framebufUnfilteredDirect;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 13, r32ui) uniform uimage2D framebufUnfilteredSpecular;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 14, rgba16f) uniform image2D framebufUnfilteredIndirectSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 15, rgba16f) uniform image2D framebufUnfilteredIndirectSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 16, rgba16f) uniform image2D framebufUnfilteredIndirectSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 17, rgba32f) uniform image2D framebufSurfacePosition;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 18, rgba32f) uniform image2D framebufVisibilityBuffer;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 19, rgba32f) uniform image2D framebufVisibilityBuffer_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 20, r16ui) uniform uimage2D framebufSectorIndex;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 21, r16ui) uniform uimage2D framebufSectorIndex_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 22, rgba16f) uniform image2D framebufViewDirection;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 23, rg32ui) uniform uimage2D framebufPrimaryToReflRefr;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 24, rgba16f) uniform image2D framebufThroughput;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 25, r11f_g11f_b10f) uniform image2D framebufPreFinal;
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 26, r11f_g11f_b10f) uniform image2D framebufFinal;
#endif
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 27, r11f_g11f_b10f) uniform image2D framebufUpscaledPing;
#endif
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 28, r11f_g11f_b10f) uniform image2D framebufUpscaledPong;
#endif
layout(set = DESC_SET_FRAMEBUFFERS, binding = 29, r11f_g11f_b10f) uniform image2D framebufUpscaledHistory;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 30, r11f_g11f_b10f) uniform image2D framebufUpscaledHistory_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 31, r32f) uniform image2D framebufDepthDlss;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 32, rg16f) uniform image2D framebufMotionDlss;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 33, r11f_g11f_b10f) uniform image2D framebufAccumHistoryLength;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 34, r11f_g11f_b10f) uniform image2D framebufAccumHistoryLength_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 35, r32ui) uniform uimage2D framebufDiffAccumColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 36, r32ui) uniform uimage2D framebufDiffAccumColor_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 37, rg16f) uniform image2D framebufDiffAccumMoments;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 38, rg16f) uniform image2D framebufDiffAccumMoments_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 39, rgba16f) uniform image2D framebufDiffColorHistory;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 40, rgba16f) uniform image2D framebufDiffPingColorAndVariance;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 41, rgba16f) uniform image2D framebufDiffPongColorAndVariance;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 42, r32ui) uniform uimage2D framebufSpecAccumColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 43, r32ui) uniform uimage2D framebufSpecAccumColor_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 44, r32ui) uniform uimage2D framebufSpecPingColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 45, r32ui) uniform uimage2D framebufSpecPongColor;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 46, rgba16f) uniform image2D framebufIndirAccumSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 47, rgba16f) uniform image2D framebufIndirAccumSH_R_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 48, rgba16f) uniform image2D framebufIndirAccumSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 49, rgba16f) uniform image2D framebufIndirAccumSH_G_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 50, rgba16f) uniform image2D framebufIndirAccumSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 51, rgba16f) uniform image2D framebufIndirAccumSH_B_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 52, rgba16f) uniform image2D framebufIndirPingSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 53, rgba16f) uniform image2D framebufIndirPingSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 54, rgba16f) uniform image2D framebufIndirPingSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 55, rgba16f) uniform image2D framebufIndirPongSH_R;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 56, rgba16f) uniform image2D framebufIndirPongSH_G;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 57, rgba16f) uniform image2D framebufIndirPongSH_B;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 58, r16f) uniform image2D framebufAtrousFilteredVariance;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 59, r11f_g11f_b10f) uniform image2D framebufBloom_Mip1;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 60, r11f_g11f_b10f) uniform image2D framebufBloom_Mip2;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 61, r11f_g11f_b10f) uniform image2D framebufBloom_Mip3;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 62, r11f_g11f_b10f) uniform image2D framebufBloom_Mip4;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 63, r11f_g11f_b10f) uniform image2D framebufBloom_Mip5;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 64, r11f_g11f_b10f) uniform image2D framebufBloom_Result;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 65, r11f_g11f_b10f) uniform image2D framebufWipeEffectSource;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 66, rgba32ui) uniform uimage2D framebufGradientSamples;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 67, rgba32ui) uniform uimage2D framebufGradientSamples_Prev;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 68, rgba16f) uniform image2D framebufDiffAndSpecPingGradient;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 69, rgba16f) uniform image2D framebufDiffAndSpecPongGradient;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 70, r16f) uniform image2D framebufIndirPingGradient;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 71, r16f) uniform image2D framebufIndirPongGradient;

// samplers
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 72) uniform sampler2D framebufAlbedo_Sampler;
#endif
layout(set = DESC_SET_FRAMEBUFFERS, binding = 73) uniform usampler2D framebufNormal_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 74) uniform usampler2D framebufNormal_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 75) uniform usampler2D framebufNormalGeometry_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 76) uniform usampler2D framebufNormalGeometry_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 77) uniform sampler2D framebufMetallicRoughness_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 78) uniform sampler2D framebufMetallicRoughness_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 79) uniform sampler2D framebufDepth_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 80) uniform sampler2D framebufDepth_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 81) uniform usampler2D framebufRandomSeed_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 82) uniform usampler2D framebufRandomSeed_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 83) uniform sampler2D framebufMotion_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 84) uniform usampler2D framebufUnfilteredDirect_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 85) uniform usampler2D framebufUnfilteredSpecular_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 86) uniform sampler2D framebufUnfilteredIndirectSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 87) uniform sampler2D framebufUnfilteredIndirectSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 88) uniform sampler2D framebufUnfilteredIndirectSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 89) uniform sampler2D framebufSurfacePosition_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 90) uniform sampler2D framebufVisibilityBuffer_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 91) uniform sampler2D framebufVisibilityBuffer_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 92) uniform usampler2D framebufSectorIndex_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 93) uniform usampler2D framebufSectorIndex_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 94) uniform sampler2D framebufViewDirection_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 95) uniform usampler2D framebufPrimaryToReflRefr_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 96) uniform sampler2D framebufThroughput_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 97) uniform sampler2D framebufPreFinal_Sampler;
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 98) uniform sampler2D framebufFinal_Sampler;
#endif
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 99) uniform sampler2D framebufUpscaledPing_Sampler;
#endif
#ifndef FRAMEBUF_IGNORE_ATTACHMENTS
layout(set = DESC_SET_FRAMEBUFFERS, binding = 100) uniform sampler2D framebufUpscaledPong_Sampler;
#endif
layout(set = DESC_SET_FRAMEBUFFERS, binding = 101) uniform sampler2D framebufUpscaledHistory_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 102) uniform sampler2D framebufUpscaledHistory_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 103) uniform sampler2D framebufDepthDlss_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 104) uniform sampler2D framebufMotionDlss_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 105) uniform sampler2D framebufAccumHistoryLength_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 106) uniform sampler2D framebufAccumHistoryLength_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 107) uniform usampler2D framebufDiffAccumColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 108) uniform usampler2D framebufDiffAccumColor_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 109) uniform sampler2D framebufDiffAccumMoments_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 110) uniform sampler2D framebufDiffAccumMoments_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 111) uniform sampler2D framebufDiffColorHistory_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 112) uniform sampler2D framebufDiffPingColorAndVariance_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 113) uniform sampler2D framebufDiffPongColorAndVariance_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 114) uniform usampler2D framebufSpecAccumColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 115) uniform usampler2D framebufSpecAccumColor_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 116) uniform usampler2D framebufSpecPingColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 117) uniform usampler2D framebufSpecPongColor_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 118) uniform sampler2D framebufIndirAccumSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 119) uniform sampler2D framebufIndirAccumSH_R_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 120) uniform sampler2D framebufIndirAccumSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 121) uniform sampler2D framebufIndirAccumSH_G_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 122) uniform sampler2D framebufIndirAccumSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 123) uniform sampler2D framebufIndirAccumSH_B_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 124) uniform sampler2D framebufIndirPingSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 125) uniform sampler2D framebufIndirPingSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 126) uniform sampler2D framebufIndirPingSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 127) uniform sampler2D framebufIndirPongSH_R_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 128) uniform sampler2D framebufIndirPongSH_G_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 129) uniform sampler2D framebufIndirPongSH_B_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 130) uniform sampler2D framebufAtrousFilteredVariance_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 131) uniform sampler2D framebufBloom_Mip1_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 132) uniform sampler2D framebufBloom_Mip2_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 133) uniform sampler2D framebufBloom_Mip3_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 134) uniform sampler2D framebufBloom_Mip4_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 135) uniform sampler2D framebufBloom_Mip5_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 136) uniform sampler2D framebufBloom_Result_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 137) uniform sampler2D framebufWipeEffectSource_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 138) uniform usampler2D framebufGradientSamples_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 139) uniform usampler2D framebufGradientSamples_Prev_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 140) uniform sampler2D framebufDiffAndSpecPingGradient_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 141) uniform sampler2D framebufDiffAndSpecPongGradient_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 142) uniform sampler2D framebufIndirPingGradient_Sampler;
layout(set = DESC_SET_FRAMEBUFFERS, binding = 143) uniform sampler2D framebufIndirPongGradient_Sampler;

// pack/unpack formats
void imageStoreUnfilteredDirect(const ivec2 pix, const vec3 unpacked) { imageStore(framebufUnfilteredDirect, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchUnfilteredDirect(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufUnfilteredDirect_Sampler, pix, 0).r); }

void imageStoreUnfilteredSpecular(const ivec2 pix, const vec3 unpacked) { imageStore(framebufUnfilteredSpecular, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchUnfilteredSpecular(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufUnfilteredSpecular_Sampler, pix, 0).r); }

void imageStoreDiffAccumColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufDiffAccumColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchDiffAccumColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufDiffAccumColor_Sampler, pix, 0).r); }
vec3 texelFetchDiffAccumColor_Prev(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufDiffAccumColor_Prev_Sampler, pix, 0).r); }

void imageStoreSpecAccumColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufSpecAccumColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchSpecAccumColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecAccumColor_Sampler, pix, 0).r); }
vec3 texelFetchSpecAccumColor_Prev(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecAccumColor_Prev_Sampler, pix, 0).r); }

void imageStoreSpecPingColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufSpecPingColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchSpecPingColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecPingColor_Sampler, pix, 0).r); }

void imageStoreSpecPongColor(const ivec2 pix, const vec3 unpacked) { imageStore(framebufSpecPongColor, pix, uvec4(encodeE5B9G9R9(unpacked))); }
vec3 texelFetchSpecPongColor(const ivec2 pix){ return decodeE5B9G9R9(texelFetch(framebufSpecPongColor_Sampler, pix, 0).r); }


#endif
