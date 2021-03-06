/**
 * Copyright (c) 2014-present, The osquery authors
 *
 * This source code is licensed as defined by the LICENSE file found in the
 * root directory of this source tree.
 *
 * SPDX-License-Identifier: (Apache-2.0 OR GPL-2.0-only)
 */

#include <osquery/utils/conversions/windows/windows_time.h>

namespace osquery {

LONGLONG filetimeToUnixtime(const FILETIME& ft) {
  LARGE_INTEGER date, adjust;
  date.HighPart = ft.dwHighDateTime;
  date.LowPart = ft.dwLowDateTime;
  adjust.QuadPart = 11644473600000 * 10000;
  date.QuadPart -= adjust.QuadPart;
  return date.QuadPart / 10000000;
}

LONGLONG longIntToUnixtime(LARGE_INTEGER& li) {
  ULARGE_INTEGER ull;
  ull.LowPart = li.LowPart;
  ull.HighPart = li.HighPart;
  return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

} // namespace osquery
