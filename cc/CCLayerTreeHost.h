// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CCLayerTreeHost_h
#define CCLayerTreeHost_h

#include "CCAnimationEvents.h"
#include "CCGraphicsContext.h"
#include "CCLayerTreeHostClient.h"
#include "CCLayerTreeHostCommon.h"
#include "CCOcclusionTracker.h"
#include "CCPrioritizedTextureManager.h"
#include "CCProxy.h"
#include "CCRenderingStats.h"
#include "IntRect.h"
#include "RateLimiter.h"
#include "SkColor.h"
#include "cc/own_ptr_vector.h"
#include <limits>
#include <wtf/HashMap.h>
#include <wtf/OwnPtr.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/PassRefPtr.h>

namespace cc {

class CCFontAtlas;
class CCLayerChromium;
class CCLayerTreeHostImpl;
class CCLayerTreeHostImplClient;
class CCPrioritizedTextureManager;
class CCTextureUpdateQueue;
class HeadsUpDisplayLayerChromium;
class Region;
struct CCScrollAndScaleSet;

struct CCLayerTreeSettings {
    CCLayerTreeSettings();
    ~CCLayerTreeSettings();

    bool acceleratePainting;
    bool showFPSCounter;
    bool showPlatformLayerTree;
    bool showPaintRects;
    bool showPropertyChangedRects;
    bool showSurfaceDamageRects;
    bool showScreenSpaceRects;
    bool showReplicaScreenSpaceRects;
    bool showOccludingRects;
    bool renderVSyncEnabled;
    double refreshRate;
    size_t maxPartialTextureUpdates;
    IntSize defaultTileSize;
    IntSize maxUntiledLayerSize;
    IntSize minimumOcclusionTrackingSize;

    bool showDebugInfo() const { return showPlatformLayerTree || showFPSCounter || showDebugRects(); }
    bool showDebugRects() const { return showPaintRects || showPropertyChangedRects || showSurfaceDamageRects || showScreenSpaceRects || showReplicaScreenSpaceRects || showOccludingRects; }
};

// Provides information on an Impl's rendering capabilities back to the CCLayerTreeHost
struct RendererCapabilities {
    RendererCapabilities();
    ~RendererCapabilities();

    GC3Denum bestTextureFormat;
    bool contextHasCachedFrontBuffer;
    bool usingPartialSwap;
    bool usingAcceleratedPainting;
    bool usingSetVisibility;
    bool usingSwapCompleteCallback;
    bool usingGpuMemoryManager;
    bool usingDiscardFramebuffer;
    bool usingEglImage;
    int maxTextureSize;
};

class CCLayerTreeHost : public RateLimiterClient {
    WTF_MAKE_NONCOPYABLE(CCLayerTreeHost);
public:
    static PassOwnPtr<CCLayerTreeHost> create(CCLayerTreeHostClient*, const CCLayerTreeSettings&);
    virtual ~CCLayerTreeHost();

    void setSurfaceReady();

    // Returns true if any CCLayerTreeHost is alive.
    static bool anyLayerTreeHostInstanceExists();

    static bool needsFilterContext() { return s_needsFilterContext; }
    static void setNeedsFilterContext(bool needsFilterContext) { s_needsFilterContext = needsFilterContext; }
    bool needsSharedContext() const { return needsFilterContext() || settings().acceleratePainting; }

    // CCLayerTreeHost interface to CCProxy.
    void willBeginFrame() { m_client->willBeginFrame(); }
    void didBeginFrame() { m_client->didBeginFrame(); }
    void updateAnimations(double monotonicFrameBeginTime);
    void layout();
    void beginCommitOnImplThread(CCLayerTreeHostImpl*);
    void finishCommitOnImplThread(CCLayerTreeHostImpl*);
    void willCommit();
    void commitComplete();
    PassOwnPtr<CCGraphicsContext> createContext();
    PassOwnPtr<CCInputHandler> createInputHandler();
    virtual PassOwnPtr<CCLayerTreeHostImpl> createLayerTreeHostImpl(CCLayerTreeHostImplClient*);
    void didLoseContext();
    enum RecreateResult {
        RecreateSucceeded,
        RecreateFailedButTryAgain,
        RecreateFailedAndGaveUp,
    };
    RecreateResult recreateContext();
    void didCommitAndDrawFrame() { m_client->didCommitAndDrawFrame(); }
    void didCompleteSwapBuffers() { m_client->didCompleteSwapBuffers(); }
    void deleteContentsTexturesOnImplThread(CCResourceProvider*);
    virtual void acquireLayerTextures();
    // Returns false if we should abort this frame due to initialization failure.
    bool initializeRendererIfNeeded();
    void updateLayers(CCTextureUpdateQueue&, size_t contentsMemoryLimitBytes);

    CCLayerTreeHostClient* client() { return m_client; }

    // Only used when compositing on the main thread.
    void composite();
    void scheduleComposite();

    // Composites and attempts to read back the result into the provided
    // buffer. If it wasn't possible, e.g. due to context lost, will return
    // false.
    bool compositeAndReadback(void *pixels, const IntRect&);

    void finishAllRendering();

    int commitNumber() const { return m_commitNumber; }

    void renderingStats(CCRenderingStats&) const;

    const RendererCapabilities& rendererCapabilities() const;

    // Test only hook
    void loseContext(int numTimes);

    void setNeedsAnimate();
    // virtual for testing
    virtual void setNeedsCommit();
    void setNeedsRedraw();
    bool commitRequested() const;

    void setAnimationEvents(PassOwnPtr<CCAnimationEventsVector>, double wallClockTime);
    virtual void didAddAnimation();

    LayerChromium* rootLayer() { return m_rootLayer.get(); }
    const LayerChromium* rootLayer() const { return m_rootLayer.get(); }
    void setRootLayer(PassRefPtr<LayerChromium>);

    const CCLayerTreeSettings& settings() const { return m_settings; }

    void setViewportSize(const IntSize& layoutViewportSize, const IntSize& deviceViewportSize);

    const IntSize& layoutViewportSize() const { return m_layoutViewportSize; }
    const IntSize& deviceViewportSize() const { return m_deviceViewportSize; }

    void setPageScaleFactorAndLimits(float pageScaleFactor, float minPageScaleFactor, float maxPageScaleFactor);

    void setBackgroundColor(SkColor color) { m_backgroundColor = color; }

    void setHasTransparentBackground(bool transparent) { m_hasTransparentBackground = transparent; }

    CCPrioritizedTextureManager* contentsTextureManager() const;

    // Delete contents textures' backing resources until they use only bytesLimit bytes. This may
    // be called on the impl thread while the main thread is running.
    void reduceContentsTexturesMemoryOnImplThread(size_t bytesLimit, CCResourceProvider*);
    // Returns true if there any evicted backing textures that have not been deleted.
    bool evictedContentsTexturesBackingsExist() const;
    // Retrieve the list of all contents textures' backings that have been evicted, to pass to the
    // main thread to unlink them from their owning textures.
    void getEvictedContentTexturesBackings(CCPrioritizedTextureManager::BackingVector&);
    // Unlink the list of contents textures' backings from their owning textures on the main thread
    // before updating layers.
    void unlinkEvictedContentTexturesBackings(const CCPrioritizedTextureManager::BackingVector&);
    // Deletes all evicted backings, unlinking them from their owning textures if needed.
    // Returns true if this function had to unlink any backings from their owning texture when
    // destroying them. If this was the case, the impl layer tree may contain invalid resources.
    bool deleteEvictedContentTexturesBackings();

    bool visible() const { return m_visible; }
    void setVisible(bool);

    void startPageScaleAnimation(const IntSize& targetPosition, bool useAnchor, float scale, double durationSec);

    void applyScrollAndScale(const CCScrollAndScaleSet&);

    void startRateLimiter(WebKit::WebGraphicsContext3D*);
    void stopRateLimiter(WebKit::WebGraphicsContext3D*);

    // RateLimitClient implementation
    virtual void rateLimit() OVERRIDE;

    bool bufferedUpdates();
    bool requestPartialTextureUpdate();
    void deleteTextureAfterCommit(PassOwnPtr<CCPrioritizedTexture>);

    void setDeviceScaleFactor(float);
    float deviceScaleFactor() const { return m_deviceScaleFactor; }

    void setFontAtlas(PassOwnPtr<CCFontAtlas>);

    HeadsUpDisplayLayerChromium* hudLayer() const { return m_hudLayer.get(); }

protected:
    CCLayerTreeHost(CCLayerTreeHostClient*, const CCLayerTreeSettings&);
    bool initialize();

private:
    typedef Vector<RefPtr<LayerChromium> > LayerList;

    void initializeRenderer();

    void update(LayerChromium*, CCTextureUpdateQueue&, const CCOcclusionTracker*);
    bool paintLayerContents(const LayerList&, CCTextureUpdateQueue&);
    bool paintMasksForRenderSurface(LayerChromium*, CCTextureUpdateQueue&);

    void updateLayers(LayerChromium*, CCTextureUpdateQueue&);

    void prioritizeTextures(const LayerList&, CCOverdrawMetrics&); 
    void setPrioritiesForSurfaces(size_t surfaceMemoryBytes);
    void setPrioritiesForLayers(const LayerList&);
    size_t calculateMemoryForRenderSurfaces(const LayerList& updateList);

    void animateLayers(double monotonicTime);
    bool animateLayersRecursive(LayerChromium* current, double monotonicTime);
    void setAnimationEventsRecursive(const CCAnimationEventsVector&, LayerChromium*, double wallClockTime);

    bool m_animating;
    bool m_needsAnimateLayers;

    CCLayerTreeHostClient* m_client;

    int m_commitNumber;
    CCRenderingStats m_renderingStats;

    OwnPtr<CCProxy> m_proxy;
    bool m_rendererInitialized;
    bool m_contextLost;
    int m_numTimesRecreateShouldFail;
    int m_numFailedRecreateAttempts;

    RefPtr<LayerChromium> m_rootLayer;
    RefPtr<HeadsUpDisplayLayerChromium> m_hudLayer;
    OwnPtr<CCFontAtlas> m_fontAtlas;

    OwnPtr<CCPrioritizedTextureManager> m_contentsTextureManager;
    OwnPtr<CCPrioritizedTexture> m_surfaceMemoryPlaceholder;

    CCLayerTreeSettings m_settings;

    IntSize m_layoutViewportSize;
    IntSize m_deviceViewportSize;
    float m_deviceScaleFactor;

    bool m_visible;

    typedef HashMap<WebKit::WebGraphicsContext3D*, RefPtr<RateLimiter> > RateLimiterMap;
    RateLimiterMap m_rateLimiters;

    float m_pageScaleFactor;
    float m_minPageScaleFactor, m_maxPageScaleFactor;
    bool m_triggerIdleUpdates;

    SkColor m_backgroundColor;
    bool m_hasTransparentBackground;

    typedef OwnPtrVector<CCPrioritizedTexture> TextureList;
    TextureList m_deleteTextureAfterCommitList;
    size_t m_partialTextureUpdateRequests;

    static bool s_needsFilterContext;
};

}

#endif
