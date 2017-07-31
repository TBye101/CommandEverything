/* Copyright (c) 2015-2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

'use strict'

var api = require('./NvCameraAPINode.node');

var _logger;

//! Helper function that receives body of POST request and calls callback for that data.
//! @param req      Request object provided by Express.
//! @param callback Callback that is triggered on succesfully downloaded data.
function getPostDataAndDo(req, callback) {
    var content = ''

    function onData(data) {
        content += data;
        _logger.info('Post Data: ' + content);
    }

    function onEnd() {
        callback(content);
    }

    req.on('data', onData);
    req.on('end', onEnd);
}

//! Helper function converts the returned error string into a JSON object
//! @param errorType      String type of error
//! @param errorString    String of error message
//! @return               JSON object of error data
function BuildErrorObject(errorType, errorString) {
    var returnCode;
    var returnText;
    var returnMessage;
    var parts = errorString.split("::");
    for (var i = 0; i < parts.length; i++) {
        var index = parts[i].search("Text:")
        if (index != -1) {
            returnText = parts[i].substring(index + 5);
        }
        index = parts[i].search("Code:")
        if (index != -1) {
            returnCode = Number(parts[i].substring(index + 5));
        }
        index = parts[i].search("Message:")
        if (index != -1) {
            returnMessage = parts[i].substring(index + 8);
        }
    }
    var errorResult = { type: errorType, code: returnCode, codeText: returnText, message: returnMessage };
    _logger.info(errorResult);
    return errorResult;
}

//! Helper function that receives JSON body of POST request and calls callback for the parsed data.
//! @param req      Request object provided by Express.
//! @param res      Response object provided by Express.
//! @param callback Callback that is triggered on succesfully parsed data.
function getJSONDataAndDo(req, res, callback) {
    function parseAndCallback(content) {
        var parsed = {};
        try {
            parsed = JSON.parse(content);
        }
        catch (e) {
            res.writeHead(400, { 'Content-Type': 'application/json' });
            var errorResult = BuildErrorObject(e.name, e.message);
            var errorString = JSON.stringify(errorResult);
            _logger.error(errorString);
            res.end(errorString);
            return;
        }
        callback(parsed);
    }
    getPostDataAndDo(req, parseAndCallback);
}

//! Formats the error and makes a reply with appropriate HTTP code.
//! @param res Response object provided by Express.
//! @param err Error object.
function replyWithError(res, err, httpCode) {
    if (httpCode) {
        res.writeHead(httpCode, { 'Content-Type': 'application/json' });
    }
    else if ('invalidArgument' in err) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
    }
    else {
        res.writeHead(500, { 'Content-Type': 'application/json' });
    }

    var errorResult = BuildErrorObject(err.name, err.message);
    var errorString = JSON.stringify(errorResult);
    _logger.error(errorString);
    res.end(errorString);
}

//! Formats the error and makes a reply with appropriate HTTP code.
//! @param res Response object provided by Express.
//! @param err Error object.
function replyWithErrorObject(res, errorObject, httpCode) {
    if (httpCode) {
        res.writeHead(httpCode, { 'Content-Type': 'application/json' });
    }
    else {
        res.writeHead(400, { 'Content-Type': 'application/json' });
    }

    var errorString = JSON.stringify(errorObject);
    _logger.error(errorString);
    res.end(errorString);
}

//! Helper function for using promises with nodebacks.
//! @param resolve The promise resolve.
//! @param reject  The promise reject.
function CallbackToPromise(resolve, reject) {
    return function (error, data) {
        if (error) {
            reject(error);
        }
        else {
            resolve(data);
        }
    }
};

function NativeCallbackToPromise(resolve, reject) {
    return function (error, data) {
        //
        // HACK: for some reasons node may delay resolving promises and calling .then
        // until next tick when reject or resolve is called from native addon.
        // This setImmediate forces execution to be continued in next tick.
        //
        if (error) {
            setImmediate((function () { reject(error); }));
        }
        else {
            setImmediate((function () { resolve(data); }));
        }
    }
};

module.exports = function (app, io, logger) {
    if (app === undefined || io === undefined || logger == undefined) {
        throw 'You need to provide express app, socket io and logging';
    }

    //! Allows global access to logging module
    _logger = logger;


    //! Gets the NVCamera availability for use
    //! Returns "{"available":"true"|"false"} for active availability
    app.post('/NvCamera/v.1.0/GetAvailable', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        try {
            api.Available(doReply, false);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    //! Gets the supported capture types as well as the currently selected type
    app.get('/NvCamera/v.1.0/Capture', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                var dataString = JSON.stringify(data);
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(dataString);
            }
        }

        try {
            api.CaptureType(doReply);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    //! Sets the capture control state
    //! Send "{"capture":"true"|"false"} for capture control enable|disable
    app.post('/NvCamera/v.1.0/Capture/Control', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.CaptureControl(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Requests the capture control state
    app.post('/NvCamera/v.1.0/Capture/GetControl', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        try {
            api.GetCaptureControl(doReply);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    //! Gets resolutions supported by a specific capture type
    app.post('/NvCamera/v.1.0/Capture/GetResolutions/:type', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        // No content is expected
        getJSONDataAndDo(req, res, function () {
            try {
                api.CaptureResolutions(doReply, req.params.type);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Gets supported filter types
    app.post('/NvCamera/v.1.0/GetFilters', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        try {
            api.FilterType(doReply, false);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    //! Sets the filter type
    //! Send "type" with a valid type string
    app.post('/NvCamera/v.1.0/Filter', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.FilterType(doReply, true, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Sets the value of an attribute of a filter type
    app.post('/NvCamera/v.1.0/Filter/:id/Attribute', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.SetFilterAttribute(doReply, req.params.id, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });


    //! Gets FX settings
    app.get('/NvCamera/v.1.0/FX', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        try {
            api.FXSettings(doReply, false);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    //! Sets FX settings
    app.post('/NvCamera/v.1.0/FX', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.FXSettings(doReply, true, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Gets FX setting ranges
    app.get('/NvCamera/v.1.0/FX/Range', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        try {
            api.FXSettingsRange(doReply, false);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    //! Gets the current camera adjustments
    app.post('/NvCamera/v.1.0/Camera/GetAdjust', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }


        getJSONDataAndDo(req, res, function (content) {
            try {
                api.CameraPosition(doReply, false, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Adjustment camera settings
    app.post('/NvCamera/v.1.0/Camera/Adjust', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.CameraPosition(doReply, true, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Requests the camera ranges for roll and FOV
    app.post('/NvCamera/v.1.0/Camera/GetRange', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.CameraSettingsRange(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

   //! Captures a screenshot with the current settings
    app.post('/NvCamera/v.1.0', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.CaptureScreenshot(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Cancels an in-progress screenshot
    app.post('/NvCamera/v.1.0/Cancel', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200)
                res.end(JSON.stringify(data));
            }
        }

        try {
            api.CaptureScreenshotCancel(doReply);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    /////////////////////////////////////////////////////////////////////////////////
    // Async Notification handling

    function EmitNotification(name, data) {
        setImmediate(function () { io.emit(name, data); });
    }

    function NotificationCallback(data) {
        logger.info('Notification with data:' + JSON.stringify(data));
        EmitNotification('/NvCamera/v.1.0/Notifications', data);
    }
    api.SetNotificationCallback(NotificationCallback);

    //! Returns the version of this module
    //! This must be the last declaration in the file. Anything below it will be masked.
    return {
        version: function () {
            return api.GetVersion();
        }
    };
};
