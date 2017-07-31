/* Copyright (c) 2015-2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

'use strict'


//! Helper function that receives body of POST request and calls callback for that data.
//! @param req      Request object provided by Express.
//! @param callback Callback that is triggered on succesfully downloaded data.
function getPostDataAndDo(req, callback) {
    var content = ''

    function onData(data) {
        content += data;
    }

    function onEnd() {
        callback(content);
    }

    req.on('data', onData);
    req.on('end', onEnd);
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
            res.writeHead(400, { 'Content-Type': 'text/html;charset=utf-8' });
            res.end(e.name + ': ' + e.message);
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
        res.writeHead(httpCode, { 'Content-Type': 'text/html;charset=utf-8' });
    }
    else if ('invalidArgument' in err) {
        res.writeHead(400, { 'Content-Type': 'text/html;charset=utf-8' });
    }
    else {
        res.writeHead(500, { 'Content-Type': 'text/html;charset=utf-8' });
    }
    res.end(err.name + ': ' + err.message);
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


module.exports = function (httpServer, app, io, logger) {
    if (app === undefined || io === undefined || logger === undefined) {
        throw 'You need to provide express app, socket io and logger';
    }

    logger.debug('Starting NvBackendAPI.js initialization');

    var https = require('https');
    logger.debug('NvBackendAPI.js: https dependency loaded');

    var fs = require('fs');
    logger.debug('NvBackendAPI.js: fs dependency loaded');

    var api = require('./NvBackendAPINode.node');
    logger.debug('NvBackendAPI.js: native dependency loaded');

    var autoDriverDownloadCallback;


    function onGetApplications(req, res) {

        function doReply(err, apps) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(apps));
            }
        }

        try {
            api.GetApplications(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    function onGetApplicationsCMS(req, res) {

        function GetFallbackCountryCode() {
            // Fallback to Ireland per https://docs.google.com/document/d/118t4kxkU4Iwd6CQ08ooa_7mYRkif9Js2nP_Bww_MgaE
            return 'IE';
        }
        function GetFallbackLanguageCode() {
            // Fallback English/Ireland per https://docs.google.com/document/d/118t4kxkU4Iwd6CQ08ooa_7mYRkif9Js2nP_Bww_MgaE
            return 'en_IE';
        }

        function GetCmsServer() {
            return 'https://assets.nvidiagrid.net';
        }

        function formatReply(applications, language, country) {
            var ret = {};
            ret.requestStatus = {};
            ret.requestStatus.statusCode = 0;
            ret.requestStatus.statusDescription = '';
            ret.requestStatus.requestId = '';
            ret.requestStatus.serverId = '';

            var cmsServer = GetCmsServer();

            //
            // From //sw/gcomp/dev/common/protocols/json/schema_gsapi_v2/js_applist.schema.json:
            // "localizedGenreSpecUrl":{
            //     "description":"URL that contains JSON with translations for genre associated with the application, url is of type: http://{domain}/meta/genres_{language code}.json",
            //     "type":"string"
            // },
            //
            ret.localizedGenreSpecUrl = cmsServer + '/tags/genres_' + language + '.json';

            //
            // From //sw/gcomp/dev/common/protocols/json/schema_gsapi_v2/js_applist.schema.json:
            // "localizedRatingSpecUrl":{
            //     "description":"URL that contains JSON with translations for ratings associated with the application, url is of type http://{domain}/rating/{county code}/rating_{language code}.json",
            //     "type":"string"
            // },
            //
            ret.localizedRatingSpecUrl = cmsServer + '/rating/' + country + '/rating_' + language + '.json';

            //
            // From //sw/gcomp/dev/common/protocols/json/schema_gsapi_v2/js_applist.schema.json:
            // "localizedTagSpecUrl":{
            //     "description":"URL that contains JSON with translations for tags associated with the application, url is of type http://{domain}/meta/tags_{language code}.json",
            //     "type":"string"
            // },
            //
            ret.localizedTagSpecUrl = cmsServer + '/tags/tags_' + language + '.json';

            ret.apps = [];

            for (var i = 0; i < applications.length; i++) {

                if (!applications[i].cmsId || !applications[i].cmsVersion) {
                    //
                    // Skipping applications without CMS support.
                    //
                    continue;
                }

                var a = {};

                a.appId = applications[i].cmsId;
                a.version = applications[i].cmsVersion;
                a.isEntitled = true;
                a.languageMetaData = {};
                a.languageMetaData.language = lang;
                a.languageMetaData.url = cmsServer + '/apps/' + applications[i].cmsId + '/' + applications[i].cmsId + '_' + language + '.json';

                a.countryMetaData = {};
                a.countryMetaData.country = country;
                a.countryMetaData.url = cmsServer + '/apps/' + applications[i].cmsId + '/' + country + '/' + applications[i].cmsId + '_' + country + '.json';

                a.appMetaDataUrl = cmsServer + '/apps/' + applications[i].cmsId + '/' + applications[i].cmsId + '_meta.json';

                a.opsSupport = applications[i].regularSupported;
                a.sopsSupport = applications[i].streamingSupported;

                a.appOperationalData = {}; // Bojan proposed to leave appOperationalData empty.

                a.backendExtras = [];
                a.backendExtras.push({ 'key': 'id', 'value': applications[i].id.toString() });
                a.backendExtras.push({ 'key': 'shortName', 'value': applications[i].shortName });
                a.backendExtras.push({ 'key': 'displayName', 'value': applications[i].displayName });
                a.backendExtras.push({ 'key': 'installDirectory', 'value': applications[i].installDirectory });

                ret.apps.push(a);
            }
            return ret;
        }

        function chooseLanguage(lang, list) {
            for (var i = 0; i < list.length; i++) {
                if (lst[i].code === lang) {
                    return lang;
                }
            }
            return GetFallbackLanguageCode();
        }

        function doReply(result) {
            var reply = {};

            var lng;
            try {
                lng = chooseLanguage(req.query.lang, result[3].languages);
            }
            catch (e) {
                lng = GetFallbackLanguageCode();
            }
            try {
                reply = formatReply(result[0], lng, result[1]);
            }
            catch (e) {
                replyWithError(res, e);
                logger.error(e);
            }

            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(reply));
        }

        function getApplicationsPromise(resolve, reject) {
            try {
                api.GetApplications(NativeCallbackToPromise(resolve, reject));
            }
            catch (err) {
                logger.error(err);
                reject(err);
            }
        };

        function getLastGeoLocationPromise(resolve, reject) {
            try {
                api.GetLastGeoLocation(NativeCallbackToPromise(resolve, reject));
            }
            catch (err) {
                logger.error(err);
                reject(err);
            }
        };

        function getSupportedLanguagesPromise(resolve, reject) {

            var emptyLanguageList = { languages: [] };

            var url = GetCmsServer() + '/locales/language_list.json';

            var r = https.request(url, function (response) {
                if (response.statusCode / 100 != 2) {
                    resolve(emptyLanguageList);
                    return;
                }
                var str = '';
                response.on('data', function (chunk) {
                    str += chunk;
                });
                response.on('end', function () {
                    try {
                        resolve(JSON.parse(str));
                    }
                    catch (e) {
                        resolve(emptyLanguageList);
                    }
                });
            });

            r.on('error', function (error) {
                resolve(emptyLanguageList);
            });

            r.end();
        }

        function getSupportedCountriesPromise(resolve, reject) {

            var url = GetCmsServer() + '/locales/country_list.json';

            var r = https.request(url, function (response) {
                if (response.statusCode / 100 != 2) {
                    reject(Error('HTTP status code is ' + response.statusCode));
                    return;
                }
                var str = '';
                response.on('data', function (chunk) {
                    str += chunk;
                });
                response.on('end', function () {
                    try {
                        resolve(JSON.parse(str));
                    }
                    catch (e) {
                        reject(Error('Response parse error: ' + e));
                    }
                });
            });

            r.on('error', function (error) {
                reject(error);
            });

            r.end();
        }

        function getCountryPromise(resolve, reject) {
            function fallbackToDefaultCountry(error) {
                resolve(GetFallbackCountryCode());
            }
            function chooseCountry(result) {
                var cc = result[0].country;
                var lst = result[1].countries;
                for (var i = 0; i < lst.length; i++) {
                    if (lst[i].code === cc) {
                        resolve(cc);
                        return;
                    }
                }
                resolve(GetFallbackCountryCode());
            }
            Promise.all([new Promise(getLastGeoLocationPromise), new Promise(getSupportedCountriesPromise)]).then(chooseCountry, fallbackToDefaultCountry).catch(fallbackToDefaultCountry);
        }

        function doErrorReply(error) {
            return replyWithError(res, error);
        }

        var lang = req.query.lang;
        if (!lang) {
            var err = new Error('"lang" argument is required');
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        var promises = [new Promise(getApplicationsPromise), new Promise(getCountryPromise), new Promise(getSupportedLanguagesPromise)];
        Promise.all(promises).then(doReply, doErrorReply).catch(doErrorReply);
    }

    app.get('/Applications/v.1.0/', onGetApplications);
    app.get('/Applications/v.1.0/CMS', onGetApplicationsCMS);

    function onPostApplicationsRefresh(req, res) {
        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        try {
            api.Scan(doReply, req.query.type);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.post('/Applications/v.1.0/refresh', onPostApplicationsRefresh);

    function onPostApplicationsRefreshStates(req, res) {
        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        try {
            api.RefreshApplicationsState(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.post('/Applications/v.1.0/refreshStates', onPostApplicationsRefreshStates);

    function onGetLastApplicationScanTime(req, res) {

        function doReply(err, t) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                if (t) {
                    res.writeHead(200, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify(t));
                }
                else {
                    res.writeHead(404, { 'Content-Type': 'text/html;charset=utf-8' });
                    res.end('There were no application scans');
                }
            }
        }

        try {
            api.GetLastApplicationScanTime(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/Applications/v.1.0/lastRefreshTime', onGetLastApplicationScanTime);

    function onPostApplicationsLaunch(req, res) {
        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        try {
            api.LaunchApplication(doReply, parseInt(req.params.id));
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.post('/Applications/v.1.0/:id/launch', onPostApplicationsLaunch);

    function onGetApplicationsSliderSettings(req, res) {

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
            api.GetSliderSettings(doReply, parseInt(req.params.id));
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/Applications/v.1.0/:id/sliderSettings', onGetApplicationsSliderSettings);

    function onGetApplicationsSettingsSpace(req, res) {
        function doReply(err, data) {
            //additional check for bug 200230315
            if (data && (typeof data === 'object') && data['settings'] && (data['settings'] instanceof Array) && data['settings'].length > 0) {
                logger.debug("GetSettingsSpace was successful for id " + req.params.id);
            }
            else {
                logger.error("GetSettingsSpace returned invalid result for id " + req.params.id + ": " + JSON.stringify(data));
            }
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        var tweak = {}
        try {
            tweak = JSON.parse(req.params.tweak);
        }
        catch (err) {
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        try {
            api.GetSettingsSpace(doReply, parseInt(req.params.id), tweak);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/Applications/v.1.0/:id/settingsSpace/:tweak', onGetApplicationsSettingsSpace);

    function onGetApplicationsState(req, res) {

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
            api.GetApplicationState(doReply, parseInt(req.params.id));
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/Applications/v.1.0/:id/state', onGetApplicationsState);

    function onPostApplicationsTargetACPosition(req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.SetTargetACPosition(doReply, parseInt(req.params.id), content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    }

    app.post('/Applications/v.1.0/:id/targetACPosition', onPostApplicationsTargetACPosition);

    function onPostApplicationsTargetDCPosition(req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.SetTargetDCPosition(doReply, parseInt(req.params.id), content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    }

    app.post('/Applications/v.1.0/:id/targetDCPosition', onPostApplicationsTargetDCPosition);

    function onGetApplicationsOpsAutoApplyEnabled(req, res) {

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
            api.GetOpsAutoApplyEnabled(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/Applications/v.1.0/OpsAutoApplyEnabled', onGetApplicationsOpsAutoApplyEnabled);

    function onPostApplicationsOpsAutoApplyEnabled(req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.SetOpsAutoApplyEnabled(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    }

    app.post('/Applications/v.1.0/OpsAutoApplyEnabled', onPostApplicationsOpsAutoApplyEnabled);

    function onGetLauncherPid(req, res) {

        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ 'pid': data }));
            }
        }

        try {
            api.LauncherPid_Get(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/Applications/v.1.0/LauncherPID', onGetLauncherPid);

    ////////////////////////////////////////////////////////////////////////////////

    function onGetSupportedApplications(req, res) {

        function doReply(err, apps) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(apps));
            }
        }

        try {
            api.GetSupportedApplications(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/SupportedApplications/v.1.0/', onGetSupportedApplications);

    ////////////////////////////////////////////////////////////////////////////////

    function onGetSettingsTranslation(req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        var shortname = req.query.shortname;
        if (!shortname) {
            var err = new Error('"shortname" argument is required');
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        var locale = req.query.locale;
        if (!locale) {
            var err = new Error('"locale" argument is required');
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        try {
            api.GetTranslation(doReply, shortname, locale);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/ApplicationSettingsTranslation/v.1.0/', onGetSettingsTranslation);

    ////////////////////////////////////////////////////////////////////////////////

    app.get('/FramerateLimiter/v.0.1/support', function (req, res) {

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
            api.GetFramerateLimiterSupport(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    app.get('/FramerateLimiter/v.0.1/state', function (req, res) {

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
            api.GetFramerateLimiterState(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    app.post('/FramerateLimiter/v.0.1/state', function (req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.SetFramerateLimiterState(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    ////////////////////////////////////////////////////////////////////////////////

    app.get('/VisualOPS/v.1.0/:shortname', function (req, res) {

        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                if (data.ready) {
                    data.manifestURL = 'http://localhost:' + httpServer.address().port + '/VisualOPS/v.1.0/' + req.params.shortname + '/manifest.json';
                }
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        if (!req.params.shortname) {
            var err = new Error('"shortname" argument is required');
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        try {
            api.GetVOPSStatus(doReply, req.params.shortname);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    app.get('/VisualOPS/v.1.0/:shortname/:filename', function (req, res) {

        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                for (var i = 0; i < data.settingInfo.length; i++) {
                    data.settingInfo[i].image = 'http://localhost:' + httpServer.address().port + '/VisualOPS/v.1.0/' + req.params.shortname + '/' + data.settingInfo[i].image;
                }
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        function onPathAvailable(err, path) {
            if (err) {
                replyWithError(res, err, 404);
            }
            else {
                //
                // Manifest is a special case.
                //
                if (req.params.filename === 'manifest.json') {
                    var manifestPath = path + '\\manifest.xml';
                    api.LoadVOPSManifest(doReply, manifestPath);
                    return;
                }

                //
                // Everything except manifest is provided as is.
                //
                var finalPath = path + '\\' + req.params.filename;
                var fileStream = fs.createReadStream(finalPath);
                fileStream.on('open', function () {
                    fileStream.pipe(res);
                });
                fileStream.on('error', function (err) {
                    replyWithError(res, err, 404);
                });
            }
        }

        if (!req.params.shortname) {
            var err = new Error('"shortname" argument is required');
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        if (!req.params.filename) {
            var err = new Error('"filename" argument is required');
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        if (req.params.filename.indexOf("./") != -1
            || req.params.filename.indexOf("/.") != -1
            || req.params.filename.indexOf(".\\") != -1
            || req.params.filename.indexOf("\\.") != -1) {
            var err = new Error('"filename" argument is invalid');
            err.invalidArgument = true;
            replyWithError(res, err);
            return;
        }

        try {
            api.GetVOPSPath(onPathAvailable, req.params.shortname);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    ////////////////////////////////////////////////////////////////////////////////

    app.get('/DriverUpdates/v.1.0/', function (req, res) {
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
            api.GetDriverUpdates(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    app.get('/DriverUpdates/v.1.0/AutomaticDriverDownloadEnabled', function (req, res) {
        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ 'enabled': data }));
            }
        }

        try {
            api.GetAutomaticDriverDownloadEnabled(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    app.post('/DriverUpdates/v.1.0/AutomaticDriverDownloadEnabled', function (req, res) {

        var enabled;

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
                if (enabled && autoDriverDownloadCallback) {
                    autoDriverDownloadCallback();
                }
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                try {
                    enabled = content.enabled;
                } catch (e) {
                    e.invalidArgument = true;
                    replyWithError(e);
                    return;
                }
                api.SetAutomaticDriverDownloadEnabled(doReply, enabled);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    app.post('/DriverUpdates/v.1.0/refresh', function (req, res) {
        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        try {
            api.StartDriverUpdatesCheck(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    ////////////////////////////////////////////////////////////////////////////////

    app.get('/TrayIcon/v.1.0/DriverUpdateNotification', function (req, res) {

        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ 'enabled': data }));
            }
        }

        try {
            api.GetTrayDriverUpdateNotification(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    app.post('/TrayIcon/v.1.0/DriverUpdateNotification', function (req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                var enabled;
                try {
                    enabled = content.enabled;
                } catch (e) {
                    e.invalidArgument = true;
                    replyWithError(e);
                    return;
                }
                api.SetTrayDriverUpdateNotification(doReply, enabled);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    app.get('/TrayIcon/v.1.0/OPSUpdateNotification', function (req, res) {

        function doReply(err, data) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ 'enabled': data }));
            }
        }

        try {
            api.GetTrayOpsUpdateNotification(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    });

    app.post('/TrayIcon/v.1.0/OPSUpdateNotification', function (req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                var enabled;
                try {
                    enabled = content.enabled;
                } catch (e) {
                    e.invalidArgument = true;
                    replyWithError(e);
                    return;
                }
                api.SetTrayOpsUpdateNotification(doReply, enabled);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    ////////////////////////////////////////////////////////////////////////////////

    app.post('/Feedback/v.0.1/', function (req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.AddFeedback(doReply, content)
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    app.post('/Feedback/v.0.1/CoPlayFeedbackError', function (req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.CoPlayFeedbackError(doReply, content)
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    app.post('/Feedback/v.0.1/CoPlayFeedback', function (req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.CoPlayFeedback(doReply, content)
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    ////////////////////////////////////////////////////////////////////////////////

    app.post('/TargetedContent/v.0.1/Presentation', function (req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202)
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.AddTargetedContentPresentation(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    });

    app.post('/TargetedContent/v.0.1/Click', function (req, res) {
        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(202)
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.AddTargetedContentClick(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        })
    });

    ////////////////////////////////////////////////////////////////////////////////

    app.get('/HardwareInformation/v.0.1/', function (req, res) {

        function doReply(err, data) {
            if (err) {
                logger.error('Request ' + req.method + ' ' + req.originalUrl + ' failed: ' + err);
                replyWithError(res, err);
            }
            else {
                logger.debug('Request ' + req.method + ' ' + req.originalUrl + ' succeeded and driver version is ' + data.DriverVersion);
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(data));
            }
        }

        try {
            api.GetHardwareInformation(doReply);
        }
        catch (err) {
            logger.error('Request ' + req.method + ' ' + req.originalUrl + ' failed: ' + err);
            replyWithError(res, err);
            logger.error(err);
        }
    });

    ////////////////////////////////////////////////////////////////////////////////

    function onPostOOTBStatus(req, res, status) {
        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
            }
        }

        try {
            api.SetOOTBStatus(doReply, status);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    function onPostOOTBStarted(req, res) {
        onPostOOTBStatus(req, res, 1);
    }

    function onPostOOTBFinished(req, res) {
        onPostOOTBStatus(req, res, 2);
    }

    app.post('/OOTB/v.1.0/started', onPostOOTBStarted);
    app.post('/OOTB/v.1.0/finished', onPostOOTBFinished);

    ////////////////////////////////////////////////////////////////////////////////

    function onGetSearchPaths(req, res) {

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
            api.SearchPaths_Get(doReply);
        }
        catch (err) {
            replyWithError(res, err);
            logger.error(err);
        }
    }

    app.get('/SearchPaths/v.1.0/', onGetSearchPaths);

    function onPostSearchPathAdd(req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.SearchPaths_Add(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    }

    app.post('/SearchPaths/v.1.0/add', onPostSearchPathAdd);

    function onPostSearchPathRemove(req, res) {

        function doReply(err) {
            if (err) {
                replyWithError(res, err);
            }
            else {
                res.writeHead(200);
                res.end();
            }
        }

        getJSONDataAndDo(req, res, function (content) {
            try {
                api.SearchPaths_Remove(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        });
    }

    app.post('/SearchPaths/v.1.0/remove', onPostSearchPathRemove);

    ////////////////////////////////////////////////////////////////////////////////

    app.post('/SignedGPUID/v.1.0/', function (req, res) {
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
                api.GetSignedGPUID(doReply, content);
            }
            catch (err) {
                replyWithError(res, err);
                logger.error(err);
            }
        })
    });

    function EmitNotification(name, data) {
        setImmediate(function () { io.emit(name, data); });
    }

    function ScanStatusCallback(status) {
        EmitNotification('/Applications/v.1.0/refresh', status);
    }

    api.SetScanStatusCallback(ScanStatusCallback);

    function ApplicationRemovedCallback(applicationId) {
        var notification = {};
        notification[applicationId] = {};
        EmitNotification('/Applications/v.1.0/', notification);
    }

    api.SetApplicationRemovedCallback(ApplicationRemovedCallback);

    function ApplicationChangedCallback(application) {
        var notification = {};
        notification[application.id] = application;
        EmitNotification('/Applications/v.1.0/', notification);
    }

    api.SetApplicationChangedCallback(ApplicationChangedCallback);

    function DriverUpdateCheckStatusCallback(status) {
        EmitNotification('/DriverUpdates/v.1.0/refresh', status);
        if (status == 'FINISHED' && autoDriverDownloadCallback) {
            autoDriverDownloadCallback();
        }
    }

    api.SetDriverUpdateCheckStatusCallback(DriverUpdateCheckStatusCallback);

    function ApplicationStateChangedCallback(data) {
        EmitNotification('/Applications/v.1.0/state', data);
    }

    api.SetApplicationStateChangedCallback(ApplicationStateChangedCallback);

    function ApplicationNewOPSCallback(data) {
        EmitNotification('/Applications/v.1.0/opsDownloaded', data);
    }

    api.SetApplicationNewOPSCallback(ApplicationNewOPSCallback);

    function VopsReadyCallback(data) {
        EmitNotification('/VisualOPS/v.1.0/', data);
    }

    api.SetVopsReadyCallback(VopsReadyCallback);

    function getDriverUpdates() {
        return new Promise(function (resolve, reject) {

            try {
                api.GetDriverUpdates(NativeCallbackToPromise(resolve, reject));
            } catch (err) {
                logger.error(err);
                reject(err);
            }
        });
    }

    function getAutomaticDriverDownloadEnabled() {
        return new Promise(function (resolve, reject) {

            try {
                api.GetAutomaticDriverDownloadEnabled(NativeCallbackToPromise(resolve, reject));
            } catch (err) {
                logger.error(err);
                reject(err);
            }
        });
    }

    return {
        version: function () {
            return api.GetVersion();
        },
        getAutomaticDriverDownloadEnabled: getAutomaticDriverDownloadEnabled,
        getDriverUpdates: getDriverUpdates,
        reportHttpSuccess: function (callback, data) {
            return api.ReportHttpSuccess(callback, data);
        },
        reportHttpFailure: function (callback, data) {
            return api.ReportHttpFailure(callback, data);
        },
        addNodeJSCrashFeedbackSync: function (data) {
            return api.AddNodeJSCrashFeedbackSync(data);
        },
        setAutoDriverDownloadCallback: function (callback) {
            autoDriverDownloadCallback = callback;
        }
    };
};
