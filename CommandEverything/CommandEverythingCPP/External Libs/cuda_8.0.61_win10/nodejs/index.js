/* Copyright (c) 2015-2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

'use strict'

////////////////////////////////////////////////////////////////////////////////
// Common utility and checks                                                  //
////////////////////////////////////////////////////////////////////////////////

if (process.version != 'v6.7.0') {
    var err = 'nodejs version 6.7.0 is required, you are using ' + process.version;
    throw err;
}

process.env.UV_THREADPOOL_SIZE = 64;

var nvUtil = require('./NvUtil.node');
nvUtil.ClaimSingleInstance();

var fs = require('fs');
var logger = require('./Logger.js')(nvUtil, GetNvNodeAppdataDirectoryPath() + '\\nvnode.log');

function SendNodeJSExceptionFeedback(message) {
    if (NvBackendAPI && typeof NvBackendAPI.addNodeJSCrashFeedbackSync === "function") {
        var err = NvBackendAPI.addNodeJSCrashFeedbackSync(message);
        if (err) {
            logger.error("Failed to send automatic feedback about NodeJS exception, reason: " + err);
        }
    }
    else {
        logger.error("Failed to send automatic feedback about NodeJS exception, NvBackend plugin is not loaded or does not have AddFeedback functionality.");
    }
}

process.on('uncaughtException', function (err) {
    logger.error("uncaughtException handler triggered!");
    if (err.stack) {
        logger.error(err.stack);
    }
    else {
        logger.error(err);
    }

    SendNodeJSExceptionFeedback(err.toString());
    process.exit(1);
})

function GfeIsInStandbyMode() {

    //
    // Currently checking if UI was ever launched.
    //

    var path = GetNvidiaAppdataDirectoryPath() + '\\NVIDIA GeForce Experience\\CefCache';
    try {
        var st = fs.statSync(path);
        return !st.isDirectory();
    }
    catch (err) {
        return (err && err.code === 'ENOENT');
    }
}

if (GfeIsInStandbyMode()) {
    logger.info('nodejs is exiting as GFE is in standby mode');
    process.exit();
}

logger.info('Starting NvContainer Local System container...');
try {
    nvUtil.StartContainerLocalSystemService(15000);
}
catch (err) {
    logger.error(err);
    process.exit();
}
logger.info('NvContainer Local System container started');


logger.info('Loading ExpressJS dependency...');
var app = require('./node_modules/express/index.js')();
logger.info('ExpressJS ready');


logger.info('Loading HTTP dependency...');
var http = require('http');
logger.info('Creating HTTP server...');
var httpServer = http.createServer(app);
logger.info('HTTP ready');


logger.info('Loading Socket.IO dependency...');
var io = require('./node_modules/socket.io/lib/index.js')(httpServer);
logger.info('Socket.IO ready');


var securityCheckEnabled = nvUtil.IsSecurityCheckEnabled();

//
// Enabling CORS and checking security cookie.
//
app.use(function (req, res, next) {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET,POST');
    res.header('Access-Control-Allow-Headers', 'X_LOCAL_SECURITY_COOKIE, Content-Type, Content-Length');

    if (securityCheckEnabled && req.headers.x_local_security_cookie != securityCookie && !req.path.startsWith('/VisualOPS/v.1.0/')) {
        res.writeHead(403, { 'Content-Type': 'text/html;charset=utf-8' });
        res.end('Security token is invalid');
    }
    else {
        logger.debug('Incoming request: ' + req.method + ' ' + req.originalUrl);
        next();
    }
});

io.use(function (socket, next) {
    if (securityCheckEnabled && socket.handshake.query.X_LOCAL_SECURITY_COOKIE != securityCookie) {
        next(new Error('Security token is invalid'));
    }
    else {
        next();
    }
});

//
// Tracking socket.io connections.
//

//! Socket rooms of the only adapter we use.
//! Used to understand if there are any clients currently connected.
var socketRooms = {};

io.on('connection', function (socket) {
    logger.info('Socket ' + socket.id + ' connected');
    socketRooms = socket.adapter.rooms;

    socket.on('error', function (error) {
        logger.info('Socket ' + socket.id + ' error: ' + error);
    });

    socket.on('reconnect', function () {
        logger.info('Socket ' + socket.id + ' reconnected');
    });

    socket.on('reconnecting', function () {
        logger.info('Socket ' + socket.id + ' reconnecting');
    });

    socket.on('reconnect_attempt', function () {
        logger.info('Socket ' + socket.id + ' reconnect attempt');
    });

    socket.on('reconnect_error', function () {
        logger.error('Socket ' + socket.id + ' reconnect error');
    });

    socket.on('reconnect_failed', function () {
        logger.error('Socket ' + socket.id + ' reconnect failed');
    });

    socket.on('disconnect', function () {
        logger.error('Socket ' + socket.id + ' disconnected');
    });
});

////////////////////////////////////////////////////////////////////////////////
// Feature modules                                                            //
////////////////////////////////////////////////////////////////////////////////

function fileExists(path) {
    try {
        var st = fs.statSync(path);
        return st.isFile();
    }
    catch (err) {
        return !(err && err.code === 'ENOENT');
    }
}

var NvBackendAPI;
{
    logger.info('Loading NvBackendAPI module...');
    NvBackendAPI = require('./NvBackendAPI.js')(httpServer, app, io, logger);
    logger.info('NvBackendAPI module loaded');
}

var NvTelemetry;
{
    logger.info('Loading NvTelemetry module...');
    NvTelemetry = require('./NvTelemetryAPI.js')(NvBackendAPI, logger);
    logger.info('NvTelemetry module loaded');
}

var ShadowPlayAPI;
{
    logger.info('Loading ShadowPlayAPI module...');
    ShadowPlayAPI = require('./NvSpCapsAPI.js')(app, io, logger);
    logger.info('ShadowPlayAPI module loaded');
}

var DriverInstallAPI;
{
    logger.info('Loading DriverInstallAPI module...');
    DriverInstallAPI = require('./DriverInstallAPI.js')(app, io, logger);
    logger.info('DriverInstallAPI module loaded');
}

var downloaderAPI;
{
    logger.info('Loading downloaderAPI module...');
    downloaderAPI = require('./downloader.js')(app, io, logger, NvTelemetry);
    logger.info('downloaderAPI module loaded');
}

var NvGameStreamAPI;
{
    logger.info('Loading NvGameStreamAPI module...');
    NvGameStreamAPI = require('./NvGameStreamAPI.js')(app, io, logger);
    logger.info('NvGameStreamAPI module loaded');
}

var NvCommonTasks;
{
    logger.info('Loading NvCommonTasks...');
    NvCommonTasks = require('./NvCommonTasks.js')();
    logger.info('NvCommonTasks loaded');
}

var NvAutoDriverDownload;
{
    logger.info('Loading NvAutoDriverDownload module...');
    NvAutoDriverDownload = require('./NvAutoDriverDownload.js')(NvCommonTasks, NvBackendAPI, downloaderAPI, logger);
    logger.info('NvAutoDriverDownload module loaded');
}

var NvAutoGFEDownload;
{
    logger.info('Loading NvAutoDownload module...');
    NvAutoGFEDownload = require('./NvAutoDownload.js');
    NvAutoGFEDownload.setAppDataPath(GetNvNodeAppdataDirectoryPath());
    NvAutoGFEDownload.init(app, io, logger, NvTelemetry, NvCommonTasks);
    logger.info('NvAutoDownload module loaded');
}

var NvGameShareAPI;
{
    logger.info('Loading NvGameShareAPI module...');
    NvGameShareAPI = require('./NvGameShareAPI.js')(app, io, logger);
    logger.info('NvGameShareAPI module loaded');
}

var NvGalleryAPI;
{
    logger.info('Loading GalleryAPI module...');
    NvGalleryAPI = require('./NvGalleryAPI.js')(app, io, logger);
    logger.info('GalleryAPI module loaded');
}

var NvAccountAPI;
{
    logger.info('Loading AccountAPI module...');
    NvAccountAPI = require('./NvAccountAPI.js')(app, io, logger);
    logger.info('AccountAPI module loaded');
}

var NvCameraAPI;
{
    logger.info('Loading NvCameraAPI module...');
    NvCameraAPI = require('./NvCameraAPI.js')(app, io, logger);
    logger.info('NvCameraAPI module loaded');
}

////////////////////////////////////////////////////////////////////////////////
// Common endpoints                                                           //
////////////////////////////////////////////////////////////////////////////////

app.get('/version', function (req, res) {
    var data = {};
    data.node = process.version;
    data.NvBackendAPI = NvBackendAPI.version();
    data.ShadowPlayAPI = ShadowPlayAPI.version();
    data.DriverInstallAPI = DriverInstallAPI.version();
    data.downloaderAPI = downloaderAPI.version();
    data.NvGameStreamAPI = NvGameStreamAPI.version();
    data.NvCommonTasks = NvCommonTasks.version();
    data.NvAutoDriverDownload = NvAutoDriverDownload.version();
    data.NvAutoGFEDownload = NvAutoGFEDownload.version();
    data.NvGameShareAPI = NvGameShareAPI.version();
    data.NvGalleryAPI = NvGalleryAPI.version();
    data.NvAccountAPI = NvAccountAPI.version();
    data.NvCameraAPI = NvCameraAPI.version();
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data));
});

app.get('/Settings/v.1.0/Language', function (req, res) {
    var data = {};
    var languageValue = '';
    try {
        languageValue = nvUtil.GetLanguage();
        logger.info('Language:' + languageValue);
        if (languageValue != undefined) {
            data.language = languageValue;
        }
        else {
            data.language = '';
        }
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(data));
    }
    catch (err) {
        replyWithError(res, err);
    }
});

app.post('/Settings/v.1.0/Language', function (req, res) {
    var parsed = {};
    function onData(data) {
        logger.info('Data:' + data);
        try {
            parsed = JSON.parse(data);
        }
        catch (err) {
            replyWithError(res, err);
        }
    }

    function onEnd() {
        try {
            logger.info('Language:' + parsed.language);
            nvUtil.SaveLanguage(parsed.language);
            setImmediate(function () {
                io.emit('/Settings/v.1.0/Language', { language: parsed.language });
            });
            res.writeHead(200)
            res.end();
        }
        catch (err) {
            replyWithError(res, err);
        }
    }

    req.on('data', onData);
    req.on('end', onEnd);
});

app.get('/threadpool', function (req, res) {
    var data = {};
    data.size = process.env.UV_THREADPOOL_SIZE;
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data));
});

//empty get, just to check if Node is up.
app.get('/up', function (req, res) {
    res.writeHead(200, { 'Content-Type': 'application/json' , 'Cache-Control': 'no-cache'});
    res.end();
});

//! Formats the error and makes a reply with appropriate HTTP code.
//! @param res Response object provided by Express.
//! @param err Error object.
function replyWithError(res, err) {
    logger.error(err);
    if ('invalidArgument' in err) {
        res.writeHead(400, { 'Content-Type': 'text/html;charset=utf-8' });
    }
    else {
        res.writeHead(500, { 'Content-Type': 'text/html;charset=utf-8' });
    }

    var errorString = JSON.stringify(err);
    logger.error(errorString);
    res.end(errorString);
}

////////////////////////////////////////////////////////////////////////////////
// Starting up                                                                //
////////////////////////////////////////////////////////////////////////////////


logger.info('Starting HTTP server...');

var portNumber = nvUtil.GetPortOverride();
if (portNumber) {
    logger.info('Overriding port number with ' + portNumber);
} else {
    portNumber = 0;
}

httpServer.listen(portNumber, '127.0.0.1', function () {
    var host = httpServer.address().address;
    var port = httpServer.address().port;

    logger.info('Server is listening at http://%s:%s', host, port);
    CreateRunFile();
    logger.info('Initialization complete.');

    nvUtil.ConfirmInitialization();
});

//
// Create file with security current port number and cookie.
// See also http://jirasw.nvidia.com/browse/CRIMSON-978 and http://jirasw.nvidia.com/browse/CRIMSON-1205
//

var securityCookie = nvUtil.GenerateRandom(16);

function GetNvidiaAppdataDirectoryPath() {
    return nvUtil.GetLocalAppdataPath() + '\\NVIDIA Corporation';
}

function GetNvNodeAppdataDirectoryPath() {
    var path = GetNvidiaAppdataDirectoryPath();

    try {
        fs.mkdirSync(path);
    } catch (e) {
        if (e.code != 'EEXIST') throw e;
    }

    path = path + '\\NvNode';
    try {
        fs.mkdirSync(path);
    } catch (e) {
        if (e.code != 'EEXIST') throw e;
    }

    return path;
}

function CreateRunFile() {
    logger.info('Creating run file with port and security cookie.');
    var config = {};
    config.port = httpServer.address().port;
    config.secret = securityCookie;

    var path = GetNvNodeAppdataDirectoryPath();

    path = path + '\\nodejs.json';
    fs.writeFileSync(path, JSON.stringify(config));
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown logic                                                             //
////////////////////////////////////////////////////////////////////////////////

function onShutdownRequested() {
    logger.info('Received shutdown request, shutting down.');

    //
    // TODO: call modules shutdown functions here.
    //
    downloaderAPI.cleanup();
    process.exit();
};

process.on('SIGTERM', onShutdownRequested);
process.on('SIGINT', onShutdownRequested);
nvUtil.SetExitCallback(onShutdownRequested);
