// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "CCKeyframedAnimationCurve.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <public/WebTransformOperations.h>
#include <public/WebTransformationMatrix.h>
#include <wtf/OwnPtr.h>
#include <wtf/Vector.h>

using namespace cc;
using WebKit::WebTransformationMatrix;

namespace {

void expectTranslateX(double translateX, const WebTransformationMatrix& matrix)
{
    EXPECT_FLOAT_EQ(translateX, matrix.m41());
}

// Tests that a float animation with one keyframe works as expected.
TEST(CCKeyframedAnimationCurveTest, OneFloatKeyframe)
{
    OwnPtr<CCKeyframedFloatAnimationCurve> curve(CCKeyframedFloatAnimationCurve::create());
    curve->addKeyframe(CCFloatKeyframe::create(0, 2, nullptr));
    EXPECT_FLOAT_EQ(2, curve->getValue(-1));
    EXPECT_FLOAT_EQ(2, curve->getValue(0));
    EXPECT_FLOAT_EQ(2, curve->getValue(0.5));
    EXPECT_FLOAT_EQ(2, curve->getValue(1));
    EXPECT_FLOAT_EQ(2, curve->getValue(2));
}

// Tests that a float animation with two keyframes works as expected.
TEST(CCKeyframedAnimationCurveTest, TwoFloatKeyframe)
{
    OwnPtr<CCKeyframedFloatAnimationCurve> curve(CCKeyframedFloatAnimationCurve::create());
    curve->addKeyframe(CCFloatKeyframe::create(0, 2, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(1, 4, nullptr));
    EXPECT_FLOAT_EQ(2, curve->getValue(-1));
    EXPECT_FLOAT_EQ(2, curve->getValue(0));
    EXPECT_FLOAT_EQ(3, curve->getValue(0.5));
    EXPECT_FLOAT_EQ(4, curve->getValue(1));
    EXPECT_FLOAT_EQ(4, curve->getValue(2));
}

// Tests that a float animation with three keyframes works as expected.
TEST(CCKeyframedAnimationCurveTest, ThreeFloatKeyframe)
{
    OwnPtr<CCKeyframedFloatAnimationCurve> curve(CCKeyframedFloatAnimationCurve::create());
    curve->addKeyframe(CCFloatKeyframe::create(0, 2, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(1, 4, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(2, 8, nullptr));
    EXPECT_FLOAT_EQ(2, curve->getValue(-1));
    EXPECT_FLOAT_EQ(2, curve->getValue(0));
    EXPECT_FLOAT_EQ(3, curve->getValue(0.5));
    EXPECT_FLOAT_EQ(4, curve->getValue(1));
    EXPECT_FLOAT_EQ(6, curve->getValue(1.5));
    EXPECT_FLOAT_EQ(8, curve->getValue(2));
    EXPECT_FLOAT_EQ(8, curve->getValue(3));
}

// Tests that a float animation with multiple keys at a given time works sanely.
TEST(CCKeyframedAnimationCurveTest, RepeatedFloatKeyTimes)
{
    OwnPtr<CCKeyframedFloatAnimationCurve> curve(CCKeyframedFloatAnimationCurve::create());
    curve->addKeyframe(CCFloatKeyframe::create(0, 4, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(1, 4, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(1, 6, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(2, 6, nullptr));

    EXPECT_FLOAT_EQ(4, curve->getValue(-1));
    EXPECT_FLOAT_EQ(4, curve->getValue(0));
    EXPECT_FLOAT_EQ(4, curve->getValue(0.5));

    // There is a discontinuity at 1. Any value between 4 and 6 is valid.
    float value = curve->getValue(1);
    EXPECT_TRUE(value >= 4 && value <= 6);

    EXPECT_FLOAT_EQ(6, curve->getValue(1.5));
    EXPECT_FLOAT_EQ(6, curve->getValue(2));
    EXPECT_FLOAT_EQ(6, curve->getValue(3));
}


// Tests that a transform animation with one keyframe works as expected.
TEST(CCKeyframedAnimationCurveTest, OneTransformKeyframe)
{
    OwnPtr<CCKeyframedTransformAnimationCurve> curve(CCKeyframedTransformAnimationCurve::create());
    WebKit::WebTransformOperations operations;
    operations.appendTranslate(2, 0, 0);
    curve->addKeyframe(CCTransformKeyframe::create(0, operations, nullptr));

    expectTranslateX(2, curve->getValue(-1));
    expectTranslateX(2, curve->getValue(0));
    expectTranslateX(2, curve->getValue(0.5));
    expectTranslateX(2, curve->getValue(1));
    expectTranslateX(2, curve->getValue(2));
}

// Tests that a transform animation with two keyframes works as expected.
TEST(CCKeyframedAnimationCurveTest, TwoTransformKeyframe)
{
    OwnPtr<CCKeyframedTransformAnimationCurve> curve(CCKeyframedTransformAnimationCurve::create());
    WebKit::WebTransformOperations operations1;
    operations1.appendTranslate(2, 0, 0);
    WebKit::WebTransformOperations operations2;
    operations2.appendTranslate(4, 0, 0);

    curve->addKeyframe(CCTransformKeyframe::create(0, operations1, nullptr));
    curve->addKeyframe(CCTransformKeyframe::create(1, operations2, nullptr));
    expectTranslateX(2, curve->getValue(-1));
    expectTranslateX(2, curve->getValue(0));
    expectTranslateX(3, curve->getValue(0.5));
    expectTranslateX(4, curve->getValue(1));
    expectTranslateX(4, curve->getValue(2));
}

// Tests that a transform animation with three keyframes works as expected.
TEST(CCKeyframedAnimationCurveTest, ThreeTransformKeyframe)
{
    OwnPtr<CCKeyframedTransformAnimationCurve> curve(CCKeyframedTransformAnimationCurve::create());
    WebKit::WebTransformOperations operations1;
    operations1.appendTranslate(2, 0, 0);
    WebKit::WebTransformOperations operations2;
    operations2.appendTranslate(4, 0, 0);
    WebKit::WebTransformOperations operations3;
    operations3.appendTranslate(8, 0, 0);
    curve->addKeyframe(CCTransformKeyframe::create(0, operations1, nullptr));
    curve->addKeyframe(CCTransformKeyframe::create(1, operations2, nullptr));
    curve->addKeyframe(CCTransformKeyframe::create(2, operations3, nullptr));
    expectTranslateX(2, curve->getValue(-1));
    expectTranslateX(2, curve->getValue(0));
    expectTranslateX(3, curve->getValue(0.5));
    expectTranslateX(4, curve->getValue(1));
    expectTranslateX(6, curve->getValue(1.5));
    expectTranslateX(8, curve->getValue(2));
    expectTranslateX(8, curve->getValue(3));
}

// Tests that a transform animation with multiple keys at a given time works sanely.
TEST(CCKeyframedAnimationCurveTest, RepeatedTransformKeyTimes)
{
    OwnPtr<CCKeyframedTransformAnimationCurve> curve(CCKeyframedTransformAnimationCurve::create());
    // A step function.
    WebKit::WebTransformOperations operations1;
    operations1.appendTranslate(4, 0, 0);
    WebKit::WebTransformOperations operations2;
    operations2.appendTranslate(4, 0, 0);
    WebKit::WebTransformOperations operations3;
    operations3.appendTranslate(6, 0, 0);
    WebKit::WebTransformOperations operations4;
    operations4.appendTranslate(6, 0, 0);
    curve->addKeyframe(CCTransformKeyframe::create(0, operations1, nullptr));
    curve->addKeyframe(CCTransformKeyframe::create(1, operations2, nullptr));
    curve->addKeyframe(CCTransformKeyframe::create(1, operations3, nullptr));
    curve->addKeyframe(CCTransformKeyframe::create(2, operations4, nullptr));

    expectTranslateX(4, curve->getValue(-1));
    expectTranslateX(4, curve->getValue(0));
    expectTranslateX(4, curve->getValue(0.5));

    // There is a discontinuity at 1. Any value between 4 and 6 is valid.
    WebTransformationMatrix value = curve->getValue(1);
    EXPECT_TRUE(value.m41() >= 4 && value.m41() <= 6);

    expectTranslateX(6, curve->getValue(1.5));
    expectTranslateX(6, curve->getValue(2));
    expectTranslateX(6, curve->getValue(3));
}

// Tests that the keyframes may be added out of order.
TEST(CCKeyframedAnimationCurveTest, UnsortedKeyframes)
{
    OwnPtr<CCKeyframedFloatAnimationCurve> curve(CCKeyframedFloatAnimationCurve::create());
    curve->addKeyframe(CCFloatKeyframe::create(2, 8, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(0, 2, nullptr));
    curve->addKeyframe(CCFloatKeyframe::create(1, 4, nullptr));
    EXPECT_FLOAT_EQ(2, curve->getValue(-1));
    EXPECT_FLOAT_EQ(2, curve->getValue(0));
    EXPECT_FLOAT_EQ(3, curve->getValue(0.5));
    EXPECT_FLOAT_EQ(4, curve->getValue(1));
    EXPECT_FLOAT_EQ(6, curve->getValue(1.5));
    EXPECT_FLOAT_EQ(8, curve->getValue(2));
    EXPECT_FLOAT_EQ(8, curve->getValue(3));
}

// Tests that a cubic bezier timing function works as expected.
TEST(CCKeyframedAnimationCurveTest, CubicBezierTimingFunction)
{
    OwnPtr<CCKeyframedFloatAnimationCurve> curve(CCKeyframedFloatAnimationCurve::create());
    curve->addKeyframe(CCFloatKeyframe::create(0, 0, CCCubicBezierTimingFunction::create(0.25, 0, 0.75, 1)));
    curve->addKeyframe(CCFloatKeyframe::create(1, 1, nullptr));

    EXPECT_FLOAT_EQ(0, curve->getValue(0));
    EXPECT_LT(0, curve->getValue(0.25));
    EXPECT_GT(0.25, curve->getValue(0.25));
    EXPECT_FLOAT_EQ(0.5, curve->getValue(0.5));
    EXPECT_LT(0.75, curve->getValue(0.75));
    EXPECT_GT(1, curve->getValue(0.75));
    EXPECT_FLOAT_EQ(1, curve->getValue(1));
}

} // namespace
