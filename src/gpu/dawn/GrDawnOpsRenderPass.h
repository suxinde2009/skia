/*
 * Copyright 2019 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrDawnOpsRenderPass_DEFINED
#define GrDawnOpsRenderPass_DEFINED

#include "src/gpu/GrOpsRenderPass.h"

#include "include/gpu/GrTypes.h"
#include "src/gpu/GrColor.h"
#include "src/gpu/GrMesh.h"
#include "dawn/webgpu_cpp.h"

class GrDawnGpu;
class GrDawnRenderTarget;
struct GrDawnProgram;

class GrDawnOpsRenderPass : public GrOpsRenderPass {
public:
    GrDawnOpsRenderPass(GrDawnGpu*, GrRenderTarget*, GrSurfaceOrigin,
                        const LoadAndStoreInfo&, const StencilLoadAndStoreInfo&);

    ~GrDawnOpsRenderPass() override;

    void begin() override { }
    void end() override;

    wgpu::RenderPassEncoder beginRenderPass(wgpu::LoadOp colorOp, wgpu::LoadOp stencilOp);

    void inlineUpload(GrOpFlushState* state, GrDeferredTextureUploadFn& upload) override;

    void submit();

private:
    GrGpu* gpu() override;

    void setScissorState(const GrProgramInfo&);
    void applyState(GrDawnProgram*, const GrProgramInfo& programInfo);

    bool onBindPipeline(const GrProgramInfo& programInfo, const SkRect& drawBounds) override;
    void onSetScissorRect(const SkIRect&) override;
    bool onBindTextures(const GrPrimitiveProcessor&, const GrSurfaceProxy* const primProcTextures[],
                        const GrPipeline&) override;
    void onBindBuffers(const GrBuffer* indexBuffer, const GrBuffer* instanceBuffer,
                       const GrBuffer* vertexBuffer, GrPrimitiveRestart) override;
    void onDraw(int vertexCount, int baseVertex) override;
    void onDrawIndexed(int indexCount, int baseIndex, uint16_t minIndexValue,
                       uint16_t maxIndexValue, int baseVertex) override;
    void onDrawInstanced(int instanceCount, int baseInstance, int vertexCount,
                         int baseVertex) override;
    void onDrawIndexedInstanced(int indexCount, int baseIndex, int instanceCount, int baseInstance,
                                int baseVertex) override;

    void onClear(const GrFixedClip&, const SkPMColor4f& color) override;

    void onClearStencilClip(const GrFixedClip&, bool insideStencilMask) override;

    struct InlineUploadInfo {
        InlineUploadInfo(GrOpFlushState* state, const GrDeferredTextureUploadFn& upload)
                : fFlushState(state), fUpload(upload) {}

        GrOpFlushState* fFlushState;
        GrDeferredTextureUploadFn fUpload;
    };

    GrDawnGpu*                  fGpu;
    wgpu::CommandEncoder        fEncoder;
    wgpu::RenderPassEncoder     fPassEncoder;
    sk_sp<GrDawnProgram>        fCurrentProgram;
    LoadAndStoreInfo            fColorInfo;

    typedef GrOpsRenderPass     INHERITED;
};

#endif
