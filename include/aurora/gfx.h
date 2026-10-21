#ifndef AURORA_GFX_H
#define AURORA_GFX_H

#ifdef __cplusplus
#include <cstdint>

extern "C" {
#else
#include "stdint.h"
#endif

#if !defined(NDEBUG) && !defined(AURORA_GFX_DEBUG_GROUPS)
#define AURORA_GFX_DEBUG_GROUPS
#endif

void push_debug_group(const char* label);
void pop_debug_group();

typedef struct {
  uint32_t queuedPipelines;
  uint32_t createdPipelines;
  uint32_t drawCallCount;
  uint32_t mergedDrawCallCount;
  uint32_t bindGroupRebuilds;
  uint32_t pipelineRebuilds;
  uint32_t uniformRebuilds;
  uint32_t mergeBlockedFmt;
  uint32_t mergeBlockedPipeline;
  uint32_t mergeBlockedTextures;
  uint32_t mergeBlockedUniformOnly;
  uint32_t lastVertSize;
  uint32_t lastUniformSize;
  uint32_t lastIndexSize;
  uint32_t lastStorageSize;
  uint32_t lastTextureUploadSize;
} AuroraStats;

const AuroraStats* aurora_get_stats();
float aurora_get_fps();

// Host-observed successful Surface::Present return, BEFORE SaltyNX/pacing work.
// Not a physical scanout-completion timestamp or calibrated GPU duration.
// Draw counts are the worker's latest recorded counts, not a public frame-ID API.
// Pipeline counters are independently atomically sampled, asynchronous totals.
typedef struct {
  uint64_t timestampNs;
  uint64_t presentationId;
  uint32_t drawCalls;
  uint32_t mergedDrawCalls;
  uint32_t pipelinesCreated;
  uint32_t pipelinesQueued;
} AuroraPresentEvent;
typedef void (*AuroraPresentCallback)(const AuroraPresentEvent* event);
// Callback runs on the serialized presentation producer, must not block/throw,
// and must copy the event before returning. A concurrent unregister can leave
// one callback in flight; its target must outlive renderer shutdown.
void aurora_set_present_callback(AuroraPresentCallback callback);

void aurora_enable_vsync(bool enabled);

#ifdef __cplusplus
}
#endif

#endif
