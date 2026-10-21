#pragma once

#include <aurora/gfx.h>
#include <atomic>

namespace aurora::gfx {

// Single serialized presentation producer; callback may be changed by the UI.
// There is no userdata whose lifetime could end while a callback is in flight.
class PresentObserver {
public:
  bool enabled() const noexcept { return mCallback.load(std::memory_order_acquire) != nullptr; }
  void setCallback(AuroraPresentCallback callback) noexcept {
    mCallback.store(callback, std::memory_order_release);
  }
  void publish(AuroraPresentEvent event) noexcept {
    if (const auto callback = mCallback.load(std::memory_order_acquire)) {
      event.presentationId = ++mSequence;
      callback(&event);
    }
  }

private:
  static_assert(std::atomic<AuroraPresentCallback>::is_always_lock_free);
  std::atomic<AuroraPresentCallback> mCallback{nullptr};
  uint64_t mSequence = 0;
};

} // namespace aurora::gfx
