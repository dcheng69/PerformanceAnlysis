/* Copyright (C) 
 * 2025 - Clay Cheng
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#include <iostream>
#include <string>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

#define TRACE_MARKER                "/sys/kernel/debug/tracing/trace_marker"

class Trace {
  public:
    explicit Trace() : m_iTraceMarkerFd(-1), m_iPid(getpid()) {
      // Open trace_marker file for reporting trace event
      m_iTraceMarkerFd = open(TRACE_MARKER, O_WRONLY);
      if (m_iTraceMarkerFd < 0) {
        throw std::runtime_error("Failed to open trace_marker");
      }
    }

    ~Trace() {
      if (m_iTraceMarkerFd >= 0) {
        close(m_iTraceMarkerFd);
      }
      m_iPid = -1;
    }

    // Start recording
    void begin(const std::string& szEventName, pid_t iTid) {
      return begin(szEventName, iTid, getTimeStampUs());
    }

    // End recording
    void end(const std::string& szEventName, pid_t iTid) {
      return end(szEventName, iTid, getTimeStampUs());
    }

    // Disable copy and copy operations on this class to prevent duplication
    Trace(const Trace&) = delete;
    Trace& operator=(const Trace&) = delete;

    // Nested ScopeGuard class to trigger trace events
    class ScopeGuard {
      private:
        Trace& trace;
        std::string szEventName;
        pid_t m_iTid;

      public:
        ScopeGuard(Trace& trace, const std::string& szEventName)
          : trace(trace), szEventName(szEventName), m_iTid(gettid()) {
            trace.begin(szEventName, m_iTid, trace.getTimeStampUs());
          }

        ~ScopeGuard() {
          trace.end(szEventName, m_iTid, trace.getTimeStampUs());
        }
    };
  private:
    int m_iTraceMarkerFd;               // fd to /sys/kernel/debug/tracing/trace_marker
    pid_t m_iPid;                       // process ID

    // inline function to acquire timestamp
    unsigned long getTimeStampUs() const {
      auto now = std::chrono::high_resolution_clock::now();
      auto duration = now.time_since_epoch();
      return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    }

    // Start recording
    void begin(const std::string& szEventName, pid_t iTid, unsigned long timestamp) {
      if (m_iTraceMarkerFd >= 0) {
        dprintf(m_iTraceMarkerFd, "B|%d|%d|%s|%lu\n", m_iPid, iTid, szEventName.c_str(), timestamp);
      }
    }

    // End recording
    void end(const std::string& szEventName, pid_t iTid, unsigned long timestamp) {
      if (m_iTraceMarkerFd >= 0) {
        dprintf(m_iTraceMarkerFd, "E|%d|%d|%s|%lu\n", m_iPid, iTid, szEventName.c_str(), timestamp);
      }
    }
};

#ifdef TRACE_ENABLE
// Each file includes trace.h will have its own trace instance
static Trace trace;
// Macro to declare Trace::ScopeGuard instance
#define TRACE_FUNCTION() \
  Trace::ScopeGuard trace_guard(trace, __FUNCTION__);

#define TRACE_B(sz_trace_event) \
  trace.begin(sz_trace_event, gettid())

#define TRACE_E(sz_trace_event) \
  trace.end(sz_trace_event, gettid())
#else
// If tracing is disabled, define empty macros
#define TRACE_FUNCTION()
#define TRACE_B(sz_trace_event)
#define TRACE_E(sz_trace_event)
#endif
