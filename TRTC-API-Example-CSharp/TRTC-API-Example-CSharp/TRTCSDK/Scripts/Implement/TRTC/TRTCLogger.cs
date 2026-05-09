// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan
using System.IO;
using System.Runtime.CompilerServices;

namespace trtc {
  public static class TRTCLogger {
    public static void Info(string message = "",
                            [CallerMemberName] string funcName = "",
                            [CallerFilePath] string filePath = "",
                            [CallerLineNumber] int lineNumber = 0) {
      Log(TRTCLogWriteLevel.logInfo, message, filePath, lineNumber, funcName);
    }

    public static void Warning(string message = "",
                              [CallerMemberName] string funcName = "",
                              [CallerFilePath] string filePath = "",
                              [CallerLineNumber] int lineNumber = 0) {
      Log(TRTCLogWriteLevel.logWarning, message, filePath, lineNumber, funcName);
    }

    public static void Error(string message = "",
                            [CallerMemberName] string funcName = "",
                            [CallerFilePath] string filePath = "",
                            [CallerLineNumber] int lineNumber = 0) {
      Log(TRTCLogWriteLevel.logError, message, filePath, lineNumber, funcName);
    }

    public static void Fatal(string message = "",
                            [CallerMemberName] string funcName = "",
                            [CallerFilePath] string filePath = "",
                            [CallerLineNumber] int lineNumber = 0) {
      Log(TRTCLogWriteLevel.logFatal, message, filePath, lineNumber, funcName);
    }

    private static void Log(TRTCLogWriteLevel log_write_level,
                            string message,
                            string filePath,
                            int lineNumber,
                            string funcName) {
      string normalizedFilePath = filePath.Replace('\\', '/');
      string fileName = Path.GetFileName(normalizedFilePath);
      string fileNameAndLine = fileName + ":" + lineNumber.ToString();
      if (string.IsNullOrEmpty(message)) {
        message = funcName;
      }
      else {
        message = funcName + " " + message;
      }

      TRTCCloudNative.trtc_cloud_write_log(log_write_level, fileNameAndLine, "unity", message);
    }
  }
}