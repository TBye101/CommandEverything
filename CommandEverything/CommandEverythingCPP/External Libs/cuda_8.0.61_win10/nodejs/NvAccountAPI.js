/* Copyright (c) 2015-2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

'use strict'

var api = require('./NVAccountAPINode');

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

    //! Set User Token
    app.post('/Account/v.1.0/UserToken', function (req, res) {
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
                // true means "set"
                api.UserToken(doReply, true, content);
            }
            catch (err) {
                replyWithError(res, err);
            }
        })
    });

    //! Get User Token
    app.get('/Account/v.1.0/UserToken', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                var dataString = JSON.stringify(data);
                logger.info('NVA::User Token Data:' + dataString);
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(dataString);
            }
        }

        try {
            // false means "get"
            api.UserToken(doReply, false);
        }
        catch (err) {
            replyWithError(res, err);
        }
    });

    function UserTokenCallback(sessionData) {
        logger.info('User token callback data : ' + JSON.stringify(sessionData));
        io.emit('/Account/v.1.0/UserToken', sessionData);
    }
    api.SetUserTokenCallback(UserTokenCallback);

    //! Returns the version of this module
    //! This must be the last declaration in the file. Anything below it will be masked.
    return {
        version: function () {
            return api.GetVersion();
        }
    };
};
