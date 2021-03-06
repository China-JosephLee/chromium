// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/string16.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "chrome/browser/ui/find_bar/find_bar_state.h"
#include "chrome/browser/ui/find_bar/find_bar_state_factory.h"
#include "chrome/browser/ui/find_bar/find_tab_helper.h"
#include "chrome/browser/ui/tab_contents/tab_contents.h"
#include "chrome/browser/ui/tab_contents/test_tab_contents.h"
#include "chrome/common/url_constants.h"
#include "chrome/test/base/testing_profile.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/test_browser_thread.h"
#include "content/public/test/web_contents_tester.h"

using content::BrowserThread;
using content::WebContents;
using content::WebContentsTester;

// TODO(avi): Kill this when TabContents goes away.
class FindBackendTestContentsCreator {
 public:
  static TabContents* CreateTabContents(content::WebContents* contents) {
    return TabContents::Factory::CreateTabContents(contents);
  }
};

class FindBackendTest : public TabContentsTestHarness {
 public:
  FindBackendTest()
      : TabContentsTestHarness(),
        browser_thread_(BrowserThread::UI, &message_loop_) {}

 private:
  content::TestBrowserThread browser_thread_;
};

namespace {

string16 FindPrepopulateText(WebContents* contents) {
  Profile* profile = Profile::FromBrowserContext(contents->GetBrowserContext());
  return FindBarStateFactory::GetLastPrepopulateText(profile);
}

}  // end namespace

// This test takes two WebContents objects, searches in both of them and
// tests the internal state for find_text and find_prepopulate_text.
TEST_F(FindBackendTest, InternalState) {
  FindTabHelper* find_tab_helper = tab_contents()->find_tab_helper();
  // Initial state for the WebContents is blank strings.
  EXPECT_EQ(string16(), FindPrepopulateText(contents()));
  EXPECT_EQ(string16(), find_tab_helper->find_text());

  // Get another WebContents object ready.
  WebContents* contents2 =
      WebContentsTester::CreateTestWebContents(profile(), NULL);
  scoped_ptr<TabContents> tab_contents(
      FindBackendTestContentsCreator::CreateTabContents(contents2));
  FindTabHelper* find_tab_helper2 = tab_contents->find_tab_helper();

  // No search has still been issued, strings should be blank.
  EXPECT_EQ(string16(), FindPrepopulateText(contents()));
  EXPECT_EQ(string16(), find_tab_helper->find_text());
  EXPECT_EQ(string16(), FindPrepopulateText(contents2));
  EXPECT_EQ(string16(), find_tab_helper2->find_text());

  string16 search_term1 = ASCIIToUTF16(" I had a 401K    ");
  string16 search_term2 = ASCIIToUTF16(" but the economy ");
  string16 search_term3 = ASCIIToUTF16(" eated it.       ");

  // Start searching in the first WebContents, searching forwards but not case
  // sensitive (as indicated by the last two params).
  find_tab_helper->StartFinding(search_term1, true, false);

  // Pre-populate string should always match between the two, but find_text
  // should not.
  EXPECT_EQ(search_term1, FindPrepopulateText(contents()));
  EXPECT_EQ(search_term1, find_tab_helper->find_text());
  EXPECT_EQ(search_term1, FindPrepopulateText(contents2));
  EXPECT_EQ(string16(), find_tab_helper2->find_text());

  // Now search in the other WebContents, searching forwards but not case
  // sensitive (as indicated by the last two params).
  find_tab_helper2->StartFinding(search_term2, true, false);

  // Again, pre-populate string should always match between the two, but
  // find_text should not.
  EXPECT_EQ(search_term2, FindPrepopulateText(contents()));
  EXPECT_EQ(search_term1, find_tab_helper->find_text());
  EXPECT_EQ(search_term2, FindPrepopulateText(contents2));
  EXPECT_EQ(search_term2, find_tab_helper2->find_text());

  // Search again in the first WebContents, searching forwards but not case
  // find_tab_helper (as indicated by the last two params).
  find_tab_helper->StartFinding(search_term3, true, false);

  // Once more, pre-populate string should always match between the two, but
  // find_text should not.
  EXPECT_EQ(search_term3, FindPrepopulateText(contents()));
  EXPECT_EQ(search_term3, find_tab_helper->find_text());
  EXPECT_EQ(search_term3, FindPrepopulateText(contents2));
  EXPECT_EQ(search_term2, find_tab_helper2->find_text());
}
