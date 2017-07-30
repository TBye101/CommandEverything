/*!
 * Copyright (c) 2015-2016, NVIDIA CORPORATION.  All rights reserved.       
 *                                                                           
 * NVIDIA CORPORATION and its licensors retain all intellectual property     
 * and proprietary rights in and to this software, related documentation     
 * and any modifications thereto.  Any use, reproduction, disclosure or      
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
webpackJsonp([2],[function(e,r,t){"use strict";Object.defineProperty(r,"__esModule",{value:!0}),r.ngRedirectModule=void 0,t(75),t(74);var n=angular.module("redirect",["ui.router"]);n.config(["$stateProvider",function(e){var r=function(e,r){e.opener.postMessage({data:r,state:""},"*")},t=function(e){for(var r=[],t=e.split("&"),n=0;n<t.length;n++){var o=t[n].split("=");r[o[0]]=o[1]}return r};e.state("redirect_query",{url:"?anything",onEnter:["$window","$http",function(e,n){e.name="redirect";var o=e.document.URL.split("?")[1];"#"===o.slice(-1)&&(o=o.slice(0,-1));var i=t(o);r(e,i)}]}).state("redirect_error_hash",{url:"/error={errorCode}",onEnter:["$window","$stateParams",function(e,t){var n={error:t.errorCode};r(e,n)}]}).state("redirect_auth",{url:"/access_token={accessToken}",onEnter:["$window","$document",function(e,n){var o=e.document.URL.split("#")[1].substr(1),i=t(o);r(e,i)}]})}]),r.ngRedirectModule=n}]);
//# sourceMappingURL=redirect.js.map