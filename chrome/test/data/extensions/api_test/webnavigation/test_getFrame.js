// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

var URL = chrome.extension.getURL("getFrame/a.html");
var tabId = -1;
var processId = -1;

chrome.test.runTests([
  function testGetFrame() {
    chrome.tabs.create({"url": "about:blank"}, function(tab) {
      tabId = tab.id;
      var done = chrome.test.listenForever(
        chrome.webNavigation.onCommitted,
        function (details) {
          if (details.tabId != tabId)
            return;
          if (details.url != URL)
            return;
          processId = details.processId;
          chrome.webNavigation.getFrame(
              {tabId: tabId, frameId: 0, processId: processId},
              function(details) {
            chrome.test.assertEq({errorOccurred: false, url: URL},
                                 details);
            done();
          });
      });
      chrome.tabs.update(tabId, {url: URL});
    });
  },

  function testGetInvalidFrame() {
    chrome.webNavigation.getFrame(
        {tabId: tabId, frameId: 1, processId: processId},
      function (details) {
        chrome.test.assertEq(null, details);
        chrome.test.succeed();
    });
  },

  function testGetAllFrames() {
    chrome.webNavigation.getAllFrames(
        {tabId: tabId},
      function (details) {
        chrome.test.assertEq(
            [{errorOccurred: false,
              frameId: 0,
              processId: processId,
              url: URL}],
             details);
        chrome.test.succeed();
    });
  }
]);
