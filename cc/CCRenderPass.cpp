// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "CCRenderPass.h"

#include "CCLayerImpl.h"
#include "CCMathUtil.h"
#include "CCOcclusionTracker.h"
#include "CCQuadCuller.h"
#include "CCSharedQuadState.h"
#include "CCSolidColorDrawQuad.h"

using WebKit::WebTransformationMatrix;

namespace cc {

scoped_ptr<CCRenderPass> CCRenderPass::create(Id id, IntRect outputRect, const WebKit::WebTransformationMatrix& transformToRootTarget)
{
    return scoped_ptr<CCRenderPass>(new CCRenderPass(id, outputRect, transformToRootTarget));
}

CCRenderPass::CCRenderPass(Id id, IntRect outputRect, const WebKit::WebTransformationMatrix& transformToRootTarget)
    : m_id(id)
    , m_transformToRootTarget(transformToRootTarget)
    , m_outputRect(outputRect)
    , m_hasTransparentBackground(true)
    , m_hasOcclusionFromOutsideTargetSurface(false)
{
    ASSERT(id.layerId > 0);
    ASSERT(id.index >= 0);
}

CCRenderPass::~CCRenderPass()
{
}

scoped_ptr<CCRenderPass> CCRenderPass::copy(Id newId) const
{
    ASSERT(newId != m_id);

    scoped_ptr<CCRenderPass> copyPass(create(newId, m_outputRect, m_transformToRootTarget));
    copyPass->setDamageRect(m_damageRect);
    copyPass->setHasTransparentBackground(m_hasTransparentBackground);
    copyPass->setHasOcclusionFromOutsideTargetSurface(m_hasOcclusionFromOutsideTargetSurface);
    copyPass->setFilters(m_filters);
    copyPass->setBackgroundFilters(m_backgroundFilters);
    return copyPass.Pass();
}

void CCRenderPass::appendQuadsForLayer(CCLayerImpl* layer, CCOcclusionTrackerImpl* occlusionTracker, CCAppendQuadsData& appendQuadsData)
{
    const bool forSurface = false;
    CCQuadCuller quadCuller(m_quadList, m_sharedQuadStateList, layer, occlusionTracker, layer->hasDebugBorders(), forSurface);

    layer->appendQuads(quadCuller, appendQuadsData);
}

void CCRenderPass::appendQuadsForRenderSurfaceLayer(CCLayerImpl* layer, const CCRenderPass* contributingRenderPass, CCOcclusionTrackerImpl* occlusionTracker, CCAppendQuadsData& appendQuadsData)
{
    const bool forSurface = true;
    CCQuadCuller quadCuller(m_quadList, m_sharedQuadStateList, layer, occlusionTracker, layer->hasDebugBorders(), forSurface);

    bool isReplica = false;
    layer->renderSurface()->appendQuads(quadCuller, appendQuadsData, isReplica, contributingRenderPass->id());

    // Add replica after the surface so that it appears below the surface.
    if (layer->hasReplica()) {
        isReplica = true;
        layer->renderSurface()->appendQuads(quadCuller, appendQuadsData, isReplica, contributingRenderPass->id());
    }
}

void CCRenderPass::appendQuadsToFillScreen(CCLayerImpl* rootLayer, SkColor screenBackgroundColor, const CCOcclusionTrackerImpl& occlusionTracker)
{
    if (!rootLayer || !screenBackgroundColor)
        return;

    Region fillRegion = occlusionTracker.computeVisibleRegionInScreen();
    if (fillRegion.isEmpty())
        return;

    bool forSurface = false;
    CCQuadCuller quadCuller(m_quadList, m_sharedQuadStateList, rootLayer, &occlusionTracker, rootLayer->hasDebugBorders(), forSurface);

    // Manually create the quad state for the gutter quads, as the root layer
    // doesn't have any bounds and so can't generate this itself.
    // FIXME: Make the gutter quads generated by the solid color layer (make it smarter about generating quads to fill unoccluded areas).
    IntRect rootTargetRect = rootLayer->renderSurface()->contentRect();
    float opacity = 1;
    bool opaque = true;
    CCSharedQuadState* sharedQuadState = quadCuller.useSharedQuadState(CCSharedQuadState::create(rootLayer->drawTransform(), rootTargetRect, rootTargetRect, opacity, opaque));
    ASSERT(rootLayer->screenSpaceTransform().isInvertible());
    WebTransformationMatrix transformToLayerSpace = rootLayer->screenSpaceTransform().inverse();
    Vector<WebCore::IntRect> fillRects = fillRegion.rects();
    for (size_t i = 0; i < fillRects.size(); ++i) {
        // The root layer transform is composed of translations and scales only, no perspective, so mapping is sufficient.
        IntRect layerRect = CCMathUtil::mapClippedRect(transformToLayerSpace, cc::IntRect(fillRects[i]));
        // Skip the quad culler and just append the quads directly to avoid occlusion checks.
        m_quadList.append(CCSolidColorDrawQuad::create(sharedQuadState, layerRect, screenBackgroundColor));
    }
}

}
