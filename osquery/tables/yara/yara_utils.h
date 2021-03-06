/**
 * Copyright (c) 2014-present, The osquery authors
 *
 * This source code is licensed as defined by the LICENSE file found in the
 * root directory of this source tree.
 *
 * SPDX-License-Identifier: (Apache-2.0 OR GPL-2.0-only)
 */

#pragma once

#include <boost/property_tree/ptree.hpp>

#include <osquery/config/config.h>
#include <osquery/core/tables.h>
#include <osquery/filesystem/fileops.h>
#include <osquery/utils/config/default_paths.h>

#ifdef CONCAT
#undef CONCAT
#endif

#include <yara.h>

namespace pt = boost::property_tree;

namespace osquery {

const std::string kYARAHome{OSQUERY_HOME "yara/"};

void YARACompilerCallback(int error_level,
                          const char* file_name,
                          int line_number,
                          const char* message,
                          void* user_data);

Status yaraInitilize(void);

Status yaraFinalize(void);

Status compileSingleFile(const std::string& file, YR_RULES** rule);

Status compileFromString(const std::string& buffer, YR_RULES** rules);

Status handleRuleFiles(const std::string& category,
                       const pt::ptree& rule_files,
                       std::map<std::string, YR_RULES*>& rules);

/**
 * Avoid scanning files that could cause hangs or issues.
 */
bool yaraShouldSkipFile(const std::string& path, mode_t st_mode);

int YARACallback(YR_SCAN_CONTEXT* context,
                 int message,
                 void* message_data,
                 void* user_data);

/**
 * @brief A simple ConfigParserPlugin for a "yara" dictionary key.
 *
 * A straight forward ConfigParserPlugin that requests a single "yara" key.
 * This stores a rather trivial "yara" data key. The accessor will be
 * redundant since this is so simple:
 *
 * Pseudo-code:
 *   getParser("yara")->getKey("yara");
 */
class YARAConfigParserPlugin : public ConfigParserPlugin {
 public:
  /// Request a single "yara" top level key.
  std::vector<std::string> keys() const override {
    return {"yara"};
  }

  // Retrieve compiled rules.
  std::map<std::string, YR_RULES*>& rules() {
    return rules_;
  }

  Status setUp() override;

 private:
  // Store compiled rules in a map (group => rules).
  std::map<std::string, YR_RULES*> rules_;

  /// Store the signatures and file_paths and compile the rules.
  Status update(const std::string& source, const ParserConfig& config) override;
};
} // namespace osquery
