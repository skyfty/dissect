// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "CSVReader.h"

#include <stddef.h>

#include <algorithm>
#include <utility>

#include "CSVFieldParser.h"


namespace {

// Returns all the characters from the start of |input| until the first '\n',
// "\r\n" (exclusive) or the end of |input|. Cuts the returned part (inclusive
// the line breaks) from |input|. Skips blocks of matching quotes. Examples:
// old input -> returned value, new input
// "ab\ncd" -> "ab", "cd"
// "\r\n" -> "", ""
// "abcd" -> "abcd", ""
// "\r" -> "\r", ""
// "a\"\n\"b" -> "a\"\n\"b", ""
std::string_view ConsumeLine(std::string_view* input) {
  bool inside_quotes = false;
  bool last_char_was_CR = false;
  for (size_t current = 0; current < input->size(); ++current) {
    char c = (*input)[current];
    switch (c) {
      case '\n':
        if (!inside_quotes) {
          const size_t eol_start = last_char_was_CR ? current - 1 : current;
          std::string_view ret(input->data(), eol_start);
          *input = input->substr(current + 1);
          return ret;
        }
        break;
      case '"':
        inside_quotes = !inside_quotes;
        break;
      default:
        break;
    }
    last_char_was_CR = (c == '\r');
  }

  // The whole |*input| is one line.
  std::string_view ret = *input;
  *input = std::string_view();
  return ret;
}

// Created for a string with potentially multiple rows of
// comma-separated-values, iteratively returns individual fields from row after
// row.
class CSVParser {
 public:
  explicit CSVParser(std::string_view csv);

  CSVParser(const CSVParser&) = delete;
  CSVParser& operator=(const CSVParser&) = delete;

  ~CSVParser();

  // Reads and unescapes values from the next row, and writes them to |fields|.
  // Consumes the end-of-line terminator. Returns false on syntax error. The
  // input must not be empty (check with HasMoreRows() before calling).
  bool ParseNextCSVRow(std::vector<std::string>* fields);

  bool HasMoreRows() const { return !remaining_csv_piece_.empty(); }

 private:
  std::string_view remaining_csv_piece_;
};

CSVParser::CSVParser(std::string_view csv) : remaining_csv_piece_(csv) {}

CSVParser::~CSVParser() = default;

bool CSVParser::ParseNextCSVRow(std::vector<std::string>* fields) {
  fields->clear();

  CSVFieldParser parser(ConsumeLine(&remaining_csv_piece_));
  std::string_view current_field;
  while (parser.HasMoreFields()) {
    if (!parser.NextField(&current_field))
      return false;
    // TODO(crbug.com/918530): Unescape the field contents in-place, as part of
    // NextField().
    std::string field_copy(current_field);
    auto pos = field_copy.find("\"\"");
    if (pos == 0) {
      field_copy.replace(0, 2, "\"");
    }
//    base::ReplaceSubstringsAfterOffset(&field_copy, 0, "\"\"", "\"");
    fields->push_back(std::move(field_copy));
  }
  return true;
}

}  // namespace

CSVTable::CSVTable() = default;

CSVTable::~CSVTable() = default;

bool CSVTable::ReadCSV(std::string_view csv) {
  records_.clear();
  column_names_.clear();

  // Read header row.
  CSVParser parser(csv);
  if (!parser.HasMoreRows()) {
    // The empty CSV is a special case. It can be seen as having one row, with a
    // single field, which is an empty string.
    column_names_.emplace_back();
    return true;
  }
  if (!parser.ParseNextCSVRow(&column_names_))
    return false;

  // Reader data records rows.
  std::vector<std::string> fields;
  while (parser.HasMoreRows()) {
    if (!parser.ParseNextCSVRow(&fields))
      return false;
    // If there are more line-breaking characters in sequence, the row parser
    // will see an empty row in between each successive two of those. Discard
    // such results, because those are useless for importing passwords.
    if (fields.size() == 1 && fields[0].empty())
      continue;

    std::map<std::string_view, std::string> row_map;
    const size_t available_columns =
        std::min(column_names_.size(), fields.size());
    for (size_t i = 0; i < available_columns; ++i) {
      row_map[column_names_[i]] = std::move(fields[i]);
    }
    records_.push_back(std::move(row_map));
  }

  return true;
}
