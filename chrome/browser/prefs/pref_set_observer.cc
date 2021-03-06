// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/prefs/pref_set_observer.h"

#include "chrome/common/pref_names.h"
#include "chrome/browser/extensions/extension_prefs.h"
#include "content/public/browser/notification_types.h"

PrefSetObserver::PrefSetObserver(PrefService* pref_service,
                                 content::NotificationObserver* observer)
    : pref_service_(pref_service),
      observer_(observer) {
  registrar_.Init(pref_service);
}

PrefSetObserver::~PrefSetObserver() {}

void PrefSetObserver::AddPref(const std::string& pref) {
  if (!prefs_.count(pref) && pref_service_->FindPreference(pref.c_str())) {
    prefs_.insert(pref);
    registrar_.Add(pref.c_str(), this);
  }
}

void PrefSetObserver::RemovePref(const std::string& pref) {
  if (prefs_.erase(pref))
    registrar_.Remove(pref.c_str(), this);
}

bool PrefSetObserver::IsObserved(const std::string& pref) {
  return prefs_.count(pref) > 0;
}

bool PrefSetObserver::IsManaged() {
  for (PrefSet::const_iterator i(prefs_.begin()); i != prefs_.end(); ++i) {
    const PrefService::Preference* pref =
        pref_service_->FindPreference(i->c_str());
    if (pref && pref->IsManaged())
      return true;
  }
  return false;
}

// static
PrefSetObserver* PrefSetObserver::CreateProxyPrefSetObserver(
    PrefService* pref_service,
    content::NotificationObserver* observer) {
  PrefSetObserver* pref_set = new PrefSetObserver(pref_service, observer);
  pref_set->AddPref(prefs::kProxy);

  return pref_set;
}

// static
PrefSetObserver* PrefSetObserver::CreateProtectedPrefSetObserver(
    PrefService* pref_service,
    content::NotificationObserver* observer) {
  PrefSetObserver* pref_set = new PrefSetObserver(pref_service, observer);
  // Homepage.
  pref_set->AddPref(prefs::kHomePageIsNewTabPage);
  pref_set->AddPref(prefs::kHomePage);
  pref_set->AddPref(prefs::kShowHomeButton);
  // Session startup.
  pref_set->AddPref(prefs::kRestoreOnStartup);
  pref_set->AddPref(prefs::kURLsToRestoreOnStartup);
  // Pinned tabs.
  pref_set->AddPref(prefs::kPinnedTabs);
  // Extensions.
  pref_set->AddPref(extensions::ExtensionPrefs::kExtensionsPref);

  return pref_set;
}

void PrefSetObserver::Observe(int type,
                              const content::NotificationSource& source,
                              const content::NotificationDetails& details) {
  if (observer_)
    observer_->Observe(type, source, details);
}
