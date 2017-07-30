/*!
 * Copyright (c) 2015-2016, NVIDIA CORPORATION.  All rights reserved.       
 *                                                                           
 * NVIDIA CORPORATION and its licensors retain all intellectual property     
 * and proprietary rights in and to this software, related documentation     
 * and any modifications thereto.  Any use, reproduction, disclosure or      
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
webpackJsonp([4],[function(e,t,a){"use strict";Object.defineProperty(t,"__esModule",{value:!0}),t.ngRedirectModule=void 0,a(52),a(123);var r=angular.module("redirect",["ui.router"]);r.config(["$stateProvider",function(e){e.state("redirect",{url:"?data&state",onEnter:["$window","$stateParams",function(e,t){e.open("","gfeclient").postMessage({data:t.data,state:t.state},"*")}]})}]),t.ngRedirectModule=r}]);
//# sourceMappingURL=redirect.js.map