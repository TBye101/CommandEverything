'use strict';
angular.module('main.config', [])
    .constant('OSC_CONFIG', {
        "userAgent": "NVIDIAOSCClient",
        "windowName": "shareclient",
        "hangouts": false,
        "nvCamera": false,
        "jsEvents": {
            "server": "https://events.gfe.nvidia.com",
            "version": "v1.0",
            "schemaVersion": "1.1",
            "maxRetries": 2,
            "msBetweenRetries": 1000,
            "defaultTimeout": 30000,
            "msBetweenSendRequest": 5000,
            "maxEventsPerRequest": 128
        },
        "jarvis": {
            "clientId": "135333107684344109"
        },
        "connect": {
            "redirectUri": "https://rds-assets.nvidia.com/main/redirect/share-redirect.html",
            "twitch": {
                "clientId": "cxcbwgocuez8axmeqe0cmz29ivvxjt2"
            },
            "imgur": {
                "clientId": "af3ff87d603599e"
            },
            "google": {
                "clientId": "954449761280-u6t6u90f9okkk4buhesve4gfn4lrc5u4.apps.googleusercontent.com",
                "clientSecret": "QyW4x6qTK6UznjiA5ODf4pR0"
            },
            "facebook": {
                "clientId": "1679326302390196",
                "clientSecret": "5fffd31fec553f7a1b6c571572d8dc5d"
            }
        }
    })
    .constant('OSC_BUILD_INFO', {
        "oscPackageVersion": "3.1.2.31",
        "branch": "rel_03_01",
        "branchType": "rel",
        "oscClientVersion": "0.0.1",
        "gitHash": "530e1f900c",
        "buildType": "prod"
    });