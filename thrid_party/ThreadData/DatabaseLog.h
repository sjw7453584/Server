/*
 * networkd.h
 *
 *  Created on: 2011-6-24
 *      Author: albertli
 */

#ifndef DATABASE_LOG_H_
#define DATABASE_LOG_H_

#include "Log.h"

#define ShowLog		databaseLog.outShow
#define FatalLog	databaseLog.outFatal
#define WarnLog		databaseLog.outWarn
#define ErrorLog	databaseLog.outError
#define DebugLog	databaseLog.outDebug
#define InfoLog		databaseLog.outInfo
#define TraceLog	databaseLog.outTrace

extern Log	databaseLog;
bool init_databaseLog(int logLevel);
//#define ErrorLog ERROR_LOG

#endif /* DATABASE_LOG_H_ */
