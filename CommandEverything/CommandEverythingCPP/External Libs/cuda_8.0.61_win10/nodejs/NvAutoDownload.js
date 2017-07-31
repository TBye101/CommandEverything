(function(){

    'use strict'
    var downloaderAPI     = require('./Downloader.node'),
        https             = require("https"),
        _                 = require("underscore"),
        store             = require('nv-localstore'),
        fs                = require('fs');
    var Reg               = require('winreg');

    var NvAutoGFEDownloadVersion = '1.0.2';
    var downloadQueue = []; //temp queue
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
	var DOWNLOAD_TYPE = {
		OTHER: 0,
		DRIVER_DOWNLOAD: 1,
		AUTO_DRIVER_DOWNLOAD: 2,
		GFE_SELF_UPDATE: 3
	};
	var NV_ERRORCODES = {
		CHECKSUM_VERIFY_FAILS: 1001,
		SIGNATURE_VERIFICATION_FAILS: 1002,
		DISK_WRITE_FAIL: 1003
	};
	var CURLE_OK = 0;
    var nvIO;
    var WSUrlVersion = 'v1.0';
    var WSHost = 'https://services.gfe.nvidia.com/GFE/';
    var WSURLTimeout = 30 * 1000; //30sec
    var WSChannel;
    var pending_rest_responses = [];
    var clientVersion;
	var taskList = [];
    var nvAppDataPath = "";
    var GFEWSConfigFile = "config.json";
    var NvidiaPath = "NVIDIA GeForce Experience";
    var NvidiaWwwPath = NvidiaPath + "/www";
    var OSVersion = "";
    var OSArch64Bit = false;
    var Logger = undefined;
    var exitCriteria = false;
    var UrlDict = {};
    var nvTelemetry;
    var isBeta = "0";
	
    function NativeCallbackToPromise(resolve, reject) {
        return function (err, data) {
            if (data) {
                //TODO: log
                setImmediate((function () { resolve(data); }));
            }
            else {
                setImmediate((function () { reject(err); }));
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
				Logger.info('Sending Telemetry failure ', d);
				return;
			}	
			if(stats.errorCode != CURLE_OK) {
				var d = {url: url, started: stTime.toISOString(), HRESULT: stats.errorCode};
				nvTelemetry.sendDownloadFailure(d);
				Logger.info('Sending Telemetry failure ', d);
				return;
			}
			var telemetryData;
			if(stats.downloadPausedOnce == 0){
				telemetryData = {url: url, started: stTime.toISOString(), httpStatus: stats.httpResponseCode, durationMS: duration};
			} else {
				telemetryData = {url: url, started: stTime.toISOString(), httpStatus: stats.httpResponseCode, durationMS: 0};
			}
			nvTelemetry.sendDownloadSuccess(telemetryData);
			Logger.info('Sending Telemetry success ', telemetryData);
		}catch(e){
			Logger.error(e);
		}
    };
		
    function statusUpdate(status){
		if(!status) return;
        sendStats(status);

        if(status.status == DOWNLOAD_STATUS.COMPLETED && _.contains(taskList, status.taskId)){
            extractInstaller(status);
            killRequested(); //request for exit if download is completed or previously downloaded.
        }
    };

    function extractInstaller(status){
        var downloadLoc = status.downloadedLocation;
        var path = downloadLoc.substring(0, downloadLoc.lastIndexOf('\\'));
        path = path.substring(0, path.lastIndexOf('\\'));

        if (process.platform === 'win32') {
            var execFileSync = require('child_process').execFileSync;
            try{
                var buffer = execFileSync('../'+NvidiaPath+'/7z.exe',
                                          ['x', '-o'+path+'\\latest', '-y', downloadLoc], {encoding: 'utf8'});
                if(buffer.toString().indexOf('Everything is Ok') != -1){
                    fs.unlink(downloadLoc);
                    status.downloadedLocation = path + '\\latest\\setup.exe';
                }else{
                    Logger.error('Extraction didnt finish with success');
                }
            } catch(e) {
                 Logger.error('Extraction failed: ' + e);
            }
        } else {
            //res.status(400);
        }
        createUpdateFile(status);
    };

    function createUpdateFile(status){
        //access appdata and create the gfeupdate.json file with url.
        var payload = {};
        payload.url = status.downloadedLocation;
        fs.writeFileSync(nvAppDataPath + "\\gfeupdate.json", JSON.stringify(payload));
    }

    function clearPendingRequests(data){
        data = data || {};
        if(pending_rest_responses.length > 0){
             _.each(pending_rest_responses, function(res, key){
                if(data.status == DOWNLOAD_STATUS.COMPLETED && data.percentComplete == 100){
                    res.send(data.downloadedLocation); //HTTP end()
                } else{
                    res.status(404).send('');
                }
            });
        }
        pending_rest_responses = [];
    };

    function initiateGFEDownload(val){
        return new Promise(function(resolve, reject){
			try{
				downloaderAPI.startDownload(NativeCallbackToPromise(resolve, reject), val.version, val.url, DOWNLOAD_TYPE.GFE_SELF_UPDATE);
                downloadQueue.push({url: val.url, ver: val.version});
			} catch (err){
				Logger.error(err);
				reject(err);
			}
		});
    };

    function onGFEUpdateFound(data){

        try{
            data = JSON.parse(data);
        }catch(e){
            Logger.error("Update Service error: " + data);
            killRequested();
            clearPendingRequests();
            return;
        }
        if(data.url == undefined) {
            Logger.error('Invalid/Empty update Url');
            killRequested();
			clearPendingRequests();
            return;
        }

        var url = data.url;
        var ver = data.version;
        var filen = url.match(/([^\/.]+)$|([^\/]+)(\.[^\/.]+)$/)[0];
        initiateGFEDownload({filename: filen, url: url, version: ver})
            .then(function(data){
                taskList.push(data.taskId);
                clearPendingRequests(data); //Dont block the REST requests
                UrlDict[data.taskId] = {url: url, stTime: (new Date())};
                if(data.status == DOWNLOAD_STATUS.COMPLETED){
                    createUpdateFile(data);
                    killRequested();
                }
            })
            .catch(function(d){
                Logger.error('initiateGFEDownload: ' + d);
            });

        /* GFE update response is not XML anymore */
    };

    function getWSSelfUpdateUrl() {
        var betaOption = store.getValue('autoGFEbeta');
		var betaStr = "Official";
		if(betaOption === "") {
			betaOption = isBeta;
		}
		betaStr = betaOption === "1" ? "Beta" : "Official";
		store.updateValue('autoGFEbeta', betaOption);

        if(WSChannel){
            betaStr = WSChannel;
        }
        if(clientVersion){
            //version query is under discussion, using 3.0.0.0
            var params = {};
            params.version = clientVersion;
            params.channel = betaStr;
            params.osVersion = OSVersion;
            params.is64bit = OSArch64Bit;
            //'{"version": "3.0.0.0", "channel": "' + betaStr +'"}';
            return WSHost+ WSUrlVersion + '/self-update/?' + JSON.stringify(params);
        }
        return "";
    };

    function checkForGFEUpdates(){

        var auto_download = store.getValue('autoGFE');
        if(auto_download == "") auto_download = "1";
        if(auto_download !== "1"){
			clearPendingRequests();
			return; //return if check for updates is disabled
		}
        if(OSVersion === ""){
            clearPendingRequests();
			return;
        }

        var updateCheckUrl = getWSSelfUpdateUrl();
        Logger.info('checking for GFE Update... ' + updateCheckUrl);
        var req = https.get(updateCheckUrl,
                (res) => {
                    var data = "";
                    res.setEncoding('binary');
                    res.on('data', (chunk)=>{ data += chunk; });
                    res.on('end', () => {
                                            onGFEUpdateFound(data);
                                        }
                          );
                    res.on('error', (e) => {
                                                Logger.error(e);
                                           });
                    Logger.info(' OTA response statusCode: ' + res.statusCode);
                    Logger.info(' OTA response headers: ' + JSON.stringify(res.headers));
                });
        req.on('error', function(err){
            Logger.error('Update Check Http Error ' + err);
            killRequested();
            clearPendingRequests();
        });
        req.setTimeout(WSURLTimeout, function(){
            Logger.error('Update Service URL timedout');
            //exit if request has timedout
            //TODO: do a retry for 2-3 times before exiting
            killRequested();
            clearPendingRequests();
        });
    };
    
    function checkForDriverUpdates(){
        //has the user disabled auto check for drivers?

    };

    function setupDownloadCompleteCallback(){

        downloaderAPI.setDownloadStatusCallback(statusUpdate);
    };

    function setupDownloaderEndpoints(app){
        app.get('/gfeupdate/autoGFEDownload/:key', function(req, res){
            res.end(store.getValue(req.params.key));
        });
        app.post('/gfeupdate/autoGFEDownload/:key', function (req, res) {
            if (req.is('text/*')) {
                req.setEncoding('utf8');
                req.text = '';
                req.on('data', function (chunk) { req.text += chunk });
                req.on('end', function () {
                    store.updateValue(req.params.key, req.text);
                    setImmediate(function () {
                        Logger.info('Emitting ' + req.url);
                        nvIO.emit(req.url, req.text);
                    });
                    res.end(req.text);
                });
            }
        });

        //POST - Begin installation of GFE
        app.post('/gfeupdate/autoGFEInstall/', function (req, res) {
            if (req.is('text/*')) {
                req.setEncoding('utf8');
                req.text = '';
                req.on('data', function (chunk) { req.text += chunk });
                req.on('end', function () {
                    if (process.platform === 'win32') {
                        var childProc = require('child_process').exec;
                        //double quote to make up for the spaces in command/folder
                        childProc("\"" + req.text + "\"", function (err, data) {
                            //data.toString();
                        });
                        res.end();
                    } else {
                        //res.status(400);
                    }
                });
            }
        });

        //GET new GFE version (explicit)
        app.get('/gfeupdate/autoGFENewVersion/', function(req, res){
            pending_rest_responses.push(res);
            exitCriteria = false;
            checkForGFEUpdates();
        });
    };

    function killRequested(data){
        exitCriteria = true;
    };

    function getClientVersionPromise(resolve, reject) {
        var regKey = new Reg({
            hive: Reg.HKLM,
            key: '\\Software\\NVIDIA Corporation\\Global\\GFExperience'
        });
        regKey.values(function (err, items) {
            if (err) {
                Logger.error("Cannot read HKLM GFExperience key from registry" + err);
                reject(err);
            }
            else {
                for (var i = 0; i < items.length; i++) {
                    if (items[i].name === 'Version') {
                        clientVersion = items[i].value;
                        Logger.info("Read GFE version from registry: " + clientVersion);
                    }
                    else if (items[i].name === 'Architecture') {
                        if (items[i].value.toLowerCase() === 'x64') {
                            OSArch64Bit = true;
                            Logger.info("Read 64bit from registry:" + OSArch64Bit);
                        }
                    }
                }
                if (clientVersion) {
                    resolve();
                }
                else {
                    reject(new Error('"Version" key was not found'));
                }
            }
        });
    }

    function getGFE2BetaPromise(resolve, reject) {
        var regKeyHKCU = new Reg({
            hive: Reg.HKCU,
            key: '\\Software\\NVIDIA Corporation\\Global\\GFExperience'
        });
        regKeyHKCU.values(function (err, items) {
            if (err) {
                Logger.error("Cannot read HKCU GFExperience key from registry" + err);
                // Not a really hard error to DIE, go to resolve() below
            }
            else {
                for (var i = 0; i < items.length; i++) {
                    if (items[i].name === 'GFE2Beta') {
                        Logger.info("Read GFE version from registry: " + items[i].value);
                        var betaReg = parseInt(items[i].value);
                        if (betaReg == 1) {
                            isBeta = "1";
                        }
                        regKeyHKCU.remove('GFE2Beta', function error(error) {
                            if (error)
                                Logger.info('error  =' + error);
                        });
                        break;
                    }
                }
            }
            resolve();
        });
    }

    function initDownloadURLAndConfig() {
        var os = require('os');

        var rel = os.release().split('.');
        OSVersion = rel[0] + "." + (rel[1] || "");
        try{
            if(OSArch64Bit){
                NvidiaWwwPath = "../../Program Files/NVIDIA Corporation/" + NvidiaWwwPath;
                NvidiaPath = "../../Program Files/NVIDIA Corporation/" + NvidiaPath;
            }
            var config = require("./"+GFEWSConfigFile);
        }catch(e){
            Logger.error(e);
            return;
        }
        if(config.gfservices){
            WSHost = config.gfservices.server;
            WSUrlVersion = config.gfservices.version;
            if(config.gfservices.selfupdate){
                WSChannel = config.gfservices.selfupdate.channel || WSChannel;
                clientVersion = config.gfservices.selfupdate.verOverride || clientVersion;
            }
        }
    };

    module.exports = {
        init: function (app, io, logger, telemetry, NvCommonTasks) {
            nvIO = io;
            Logger = logger;
            nvTelemetry = telemetry;

            if (NvCommonTasks.setIntervalCallback) {
                NvCommonTasks.setIntervalCallback(checkForGFEUpdates);
            };


            setupDownloaderEndpoints(app);

            // Check for updates after startup.
            Promise.all([new Promise(getClientVersionPromise), new Promise(getGFE2BetaPromise)]).then(function () {
                initDownloadURLAndConfig();
                checkForGFEUpdates();
            }).catch(function (err) {
                Logger.error("Failed to perform update check " + err);
            });

            setupDownloadCompleteCallback();
        },

        stop: function () {
            _.each(downloadQueue, function (elem) {
                downloaderAPI.stopDownload(function () { }, elem.ver);
            });
        },

        version: function () {
            return NvAutoGFEDownloadVersion;
        },

        setAppDataPath: function (path) {
            nvAppDataPath = path;
        },

        canNodeExitNow: function () {
            return exitCriteria;
        }
    }
}());