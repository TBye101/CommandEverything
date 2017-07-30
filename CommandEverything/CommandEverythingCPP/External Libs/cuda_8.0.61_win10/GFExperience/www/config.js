'use strict';
angular.module('main.config', [])
    .constant('GFECLIENT_CONFIG', {
        "userAgent": "NVIDIAGFEClient",
        "windowName": "gfeclient",
        "mode": "prod",
        "jarvis": {
            "server": "https://accounts.nvgs.nvidia.com",
            "version": "1",
            "deviceId": "gfeclient",
            "clientId": "135333107684344109",
            "clientDescription": "GFE {VERSION}",
            "redirectUrl": "https://rds-assets.nvidia.com/main/redirect/gfe-redirect.html#",
            "defaultTimeout": 10000,
            "defaultRetries": 2,
            "defaultTimeBetweenRetries": 500
        },
        "gfwsl": {
            "server": "https://gfwsl.geforce.com/",
            "defaultRetries": 2,
            "defaultTimeout": 10000
        },
        "gfservices": {
            "server": "https://services.gfe.nvidia.com/GFE/",
            "version": "v1.0"
        },
        "imageFormatService": {
            "server": "https://images.nvidiagrid.net/"
        },
        "jsEvents": {
            "server": "https://events.gfe.nvidia.com",
            "version": "v1.0",
            "schemaVersion": "1.8",
            "defaultRetries": 2,
            "defaultTimeBetweenRetries": 1000,
            "defaultTimeout": 30000,
            "msBetweenSendRequest": 5000,
            "maxEventsPerRequest": 128
        },
        "cms": {
            "defaultRetries": 2
        },
        "nodeService": {
            "reconnection": true,
            "reconnectionDelay": 1000,
            "reconnectionDelayMax": 1000,
            "reconnectionAttempts": 20
        },
        "rds": {
            "clientId": "b7e85bb1-afe6-4867-b11f-a32a58b7f797",
            "portalRealm": "03a9b13f-29ca-4435-b7d9-22757bfa5b4f"
        }
    })
    .constant('GFECLIENT_BUILD_INFO', {
        "gfePackageVersion": "3.1.2.31",
        "branch": "rel_03_01",
        "branchType": "rel",
        "gfeclientVersion": "0.1.0",
        "gitHash": "0ff51fe740",
        "buildType": "prod"
    });