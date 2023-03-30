//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.h
//
// Identification: src/include/buffer/lru_replacer.h
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <list>
#include <mutex> // NOLINT
#include <vector>

#include "buffer/replacer.h"
#include "common/config.h"

namespace bustub
{

  /**
   * MRUReplacer implements the Least Recently Used replacement policy.
   */
  class MRUReplacer : public Replacer
  {
  public:
    /**
     * Create a new MRUReplacer.
     * @param num_pages the maximum number of pages the MRUReplacer will be required to store
     */
    explicit MRUReplacer(size_t num_pages);

    /**
     * Destroys the MRUReplacer.
     */
    ~MRUReplacer() override;

    auto Victim(frame_id_t *frame_id) -> bool override;

    void Pin(frame_id_t frame_id) override;

    void Unpin(frame_id_t frame_id) override;

    auto Size() -> size_t override;

  private:
    // TODO(student): implement me!
  };

} // namespace bustub
