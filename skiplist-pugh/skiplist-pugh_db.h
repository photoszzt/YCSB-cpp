#pragma once
#ifndef SKIPLISTPUGHDB_H_
#define SKIPLISTPUGHDB_H_

#include <string>
#include <vector>
#include "core/db.h"
#include "CharArray.h"
#include "skiplist-pugh/intset.h"

class SkiplistPughDB: public DB {
  using DB::Field;
  using DB::Status;

  SkiplistPughDB() {
      unsigned int levelmax = floor_log_2((unsigned int) 1024);
      unsigned int size_pad_32 = sizeof(pnode) + (levelmax * sizeof(pnode *));
      while (size_pad_32 & 31)
      {
          size_pad_32++;
      }

      skiplist_ = sl_set_new<CharArray, sl_intset<CharArray, CharArray>>(levelmax, size_pad_32);
  }
  ~SkiplistPughDB() {
      sl_set_delete(skiplist_);
  }

  Status Read(const std::string &table, const std::string &key,
          const std::vector<std::string> *fields,
          std::vector<Field> &result) override;

  Status Scan(const std::string &table, const std::string &key,
          int record_count, const std::vector<std::string> *fields,
          std::vector<std::vector<Field>> &result) override;

  Status Update(const std::string &table, const std::string &key,
          std::vector<Field> &values) override;

  Status Insert(const std::string &table, const std::string &key,
          std::vector<Field> &values) override;

  Status Delete(const std::string &table, const std::string &key) override;

  sl_intset<CharArray, sl_intset<CharArray, CharArray>>* skiplist_;
};

#endif // SKIPLISTPUGHDB_H_
