// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_IMPORT_CSV_READER_H_
#define COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_IMPORT_CSV_READER_H_

#include <map>
#include <string>
#include <vector>

// Parsed representation of tabular CSV data.
class CSVTable {
 public:
  CSVTable();

  CSVTable(const CSVTable&) = delete;
  CSVTable& operator=(const CSVTable&) = delete;

  ~CSVTable();


  bool ReadCSV(std::string_view csv);

  const std::vector<std::string>& column_names() const { return column_names_; }

  const std::vector<std::map<std::string_view, std::string>>& records() const {
    return records_;
  }

 private:
  // Values from the first row.
  std::vector<std::string> column_names_;
  // Values from the subsequent rows. Each map represents one row and maps the
  // column names to the value stored at that column in that row.
  std::vector<std::map<std::string_view, std::string>> records_;
};

#endif  // COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_IMPORT_CSV_READER_H_
