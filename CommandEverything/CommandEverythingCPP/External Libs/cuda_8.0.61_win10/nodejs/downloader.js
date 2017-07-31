/* Copyright (c) 2015-2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

'use strict'
var lastReceivedTime = undefined;
var exitCriteria = false;
var nvLogger;
var UrlDict = {};
var DOWNLOAD_STATUS  = {
	UNDEFINED: -1,
    DOWNLOADING: 0,
    PAUSED: 1,
    COMPLETED: 2,
    RETRYING: 3,
	PAUSED_FOR_FAILED: 4,
	STOPPED_FOR_FAILED: 5,
    CHECKSUM_VERIFY_FAILS: 6,
    SIGNATURE_VERIFICATION_FAILS: 7,
    DISK_WRITE_FAIL: 8,
	DOWNLOAD_ERROR: 9,
    OPERATION_IN_PROGRESS: 10
};
var NV_ERRORCODES = {
	CHECKSUM_VERIFY_FAILS: 1001,
	SIGNATURE_VERIFICATION_FAILS: 1002,
	DISK_WRITE_FAIL: 1003
};
var CURLE_OK = 0;
var nvTelemetry;

function NativeCallbackToPromise(resolve, reject) {
    return function (err, data) {
        //
        // HACK: for some reasons node may delay resolving promises and calling .then
        // until next tick when reject or resolve is called from native addon.
        // This setImmediate forces execution to be continued in next tick.
        //
		if(err){
			setImmediate((function () { reject(err); }));
		}
        else {
			setImmediate((function () { resolve(data); }));
        }
    }
};

function sendStats(stats){
    if(!nvTelemetry) return;
	try{
		var d; 
		if(!UrlDict[stats.taskId]) return;
        var url = UrlDict[stats.taskId].url;
        var stTime = UrlDict[stats.taskId].stTime;
        var duration = (new Date()).getTime() - stTime.getTime();
		switch(stats.status){
			case DOWNLOAD_STATUS.DOWNLOADING:
			case DOWNLOAD_STATUS.PAUSED:
			case DOWNLOAD_STATUS.RETRYING:
			    return;
			case DOWNLOAD_STATUS.DISK_WRITE_FAIL:
		        d = {url: url, started: stTime.toISOString(), HRESULT: NV_ERRORCODES.DISK_WRITE_FAIL};
				break;
			case DOWNLOAD_STATUS.CHECKSUM_VERIFY_FAILS:
	            d = {url: url, started: stTime.toISOString(), HRESULT: NV_ERRORCODES.CHECKSUM_VERIFY_FAILS};
				break;
			case DOWNLOAD_STATUS.SIGNATURE_VERIFICATION_FAILS:		
				d = {url: url, started: stTime.toISOString(), HRESULT: NV_ERRORCODES.SIGNATURE_VERIFICATION_FAILS};
				break;
			default:
				;
		}
		if(typeof d !== 'undefined')
		{				
			nvTelemetry.sendDownloadFailure(d);
			nvLogger.info('Sending Telemetry failure ', d);
			return;
		}
    	if(stats.errorCode != CURLE_OK) {
			var d = {url: url, started: stTime.toISOString(), HRESULT: stats.errorCode};
            nvTelemetry.sendDownloadFailure(d);
			nvLogger.info('Sending Telemetry failure ', d);
			return;
		}
		var telemetryData;
		if(stats.downloadPausedOnce == 0){
			telemetryData = {url: url, started: stTime.toISOString(), httpStatus: stats.httpResponseCode, durationMS: duration};
        } else {
			telemetryData = {url: url, started: stTime.toISOString(), httpStatus: stats.httpResponseCode, durationMS: 0};
		}
		nvTelemetry.sendDownloadSuccess(telemetryData);
		nvLogger.info('Sending Telemetry success ', telemetryData);
    }catch(e){
        nvLogger.error(e);
    }
};

module.exports = function(app, io, logger, telemetry){

	nvLogger = logger;
    nvTelemetry = telemetry;
    try {
        var downloaderApi = require('./Downloader.node');
    } catch (e) {
        nvLogger.info('Failed to load downloader Addon', e);
        return null;
    }

    function handleException(err, res, httpCode){
        if (httpCode) {
        res.writeHead(httpCode, { 'Content-Type': 'text/html;charset=utf-8' });
		}
		else if ('invalidArgument' in err) {
			res.writeHead(400, { 'Content-Type': 'text/html;charset=utf-8' });
		}
		else if ('notFound' in err) {
			res.writeHead(404, { 'Content-Type': 'text/html;charset=utf-8' });
		}
		else {
			res.writeHead(500, { 'Content-Type': 'text/html;charset=utf-8' });
		}
		res.end(err.name + ': ' + err.message);
    }

	function startDownload(version, url, downloadType){
		var args = Array.prototype.slice.call(arguments);
		return new Promise(function(resolve, reject){

			try{
				args.unshift(NativeCallbackToPromise(resolve, reject));
				downloaderApi.startDownload.apply(this, args);
			} catch (err){
				nvLogger.error(err);
				reject(err);
			}
		});
	}

    app.post('/download/v.0.1/start/:version/:url', function (req, res) {

      var version = req.params.version;
      var url = req.params.url;

      startDownload(version, url).then(function(retValue){
		  res.writeHead(200, { 'Content-Type': 'application/json' });
		  res.end(JSON.stringify(retValue));
          updateUrlDict(retValue.taskId, url, true);
	  }, function(err){
		  return handleException(err, res);
	  });
    });

	app.post('/download/v.0.1/start/:version/:url/:downloadType', function (req, res) {

      var version = req.params.version;
      var url = req.params.url;
	  var downloadType = parseInt(req.params.downloadType);

      startDownload(version, url, downloadType).then(function(retValue){
		  res.writeHead(200, { 'Content-Type': 'application/json' });
		  res.end(JSON.stringify(retValue));
          updateUrlDict(retValue.taskId, url, true);
	  }, function(err){
		  return handleException(err, res);
	  });
    });

    app.post('/download/v.0.1/pause/:taskId', function (req, res) {

      function doReply(err, retValue){
		  if(err){
			handleException(err, res);
		  }
		  else{
			res.writeHead(200, { 'Content-Type': 'application/json' });
			res.end(JSON.stringify(retValue));
		  }
      }

      var taskId = req.params.taskId;

      var retValue;
      try {
        retValue = downloaderApi.pauseDownload(doReply, taskId);
      } catch  (e) {
        return handleException(e, res);
      }
    });

    app.post('/download/v.0.1/resume/:taskId', function (req, res) {

      function doReply(err, retValue){
		  if(err){
			handleException(err, res);
		  }
		  else{
			res.writeHead(200, { 'Content-Type': 'application/json' });
			res.end(JSON.stringify(retValue));
			updateUrlDict(retValue.taskId, retValue.downloadUrl);
		  }
      }

      var taskId = req.params.taskId;

      var retValue;
      try {
        retValue = downloaderApi.resumeDownload(doReply, taskId);
      } catch  (e) {
        return handleException(e, res);
      }

    });

    app.post('/download/v.0.1/stop/:taskId', function (req, res) {

      function doReply(err, retValue){
		  if(err){
			handleException(err, res);
		  }
		  else{
			res.writeHead(200, { 'Content-Type': 'application/json' });
			res.end(JSON.stringify(retValue));
		  }
      }

      var taskId = req.params.taskId;

      var retValue;
      try {
        retValue = downloaderApi.stopDownload(doReply, taskId);
      } catch  (e) {
        return handleException(e, res);
      }

    });

	function getStatus(version, url){
		var args = Array.prototype.slice.call(arguments);
		return new Promise(function(resolve, reject){
			try{
				args.unshift(NativeCallbackToPromise(resolve, reject));
				downloaderApi.getStatus.apply(this, args);
			} catch (err){
				nvLogger.error(err);
				reject(err);
			}
		});
	}

    app.get('/download/v.0.1/status/:taskId', function (req, res) {

		var taskId = req.params.taskId;

		getStatus(taskId).then(function (retValue){
		  res.writeHead(200, { 'Content-Type': 'application/json' });
		  res.end(JSON.stringify(retValue));
		}, function(err){
		  handleException(err, res);
		});
    });

	app.get('/download/v.0.1/status/:version/:url', function (req, res) {

		var version = req.params.version;
		var url = req.params.url;

		getStatus(version, url).then(function (retValue){
		  res.writeHead(200, { 'Content-Type': 'application/json' });
		  res.end(JSON.stringify(retValue));
		}, function(err){
		  handleException(err, res);
		});
    });

    function statusUpdate(status){
		lastReceivedTime = Date.now();
		logger.info(status);
        setImmediate(function () {
            io.emit('/download/v.0.1/status', status);
        });
		sendStats(status);
    }

    downloaderApi.setDownloadStatusCallback(statusUpdate);

    function updateUrlDict(taskId, url, overwrite){
		if(overwrite || !UrlDict[taskId]){
			UrlDict[taskId] = {url: url, stTime: (new Date())};
		}
    };

    return {
        version: function () {
            return downloaderApi.getVersion();
        },
		startDownload: startDownload,
		getStatus: getStatus,
        canNodeExitNow: function(){
            return exitCriteria;
        },
        updateTaskIdForUrl: function(taskId, url){
            updateUrlDict(taskId, url);
        },
        cleanup: function () {
            downloaderApi.cleanup();
        }
    };
}