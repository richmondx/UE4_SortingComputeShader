/******************************************************************************
* The MIT License (MIT)
*
* Copyright (c) 2015 Fredrik Lindh
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
******************************************************************************/

#pragma once

#include "Private/ComputeShaderDeclaration.h"

const UINT NUM_ELEMENTS = 1024 * 1024;
const UINT BITONIC_BLOCK_SIZE = 1024;
const UINT TRANSPOSE_BLOCK_SIZE = 16;
const UINT MATRIX_WIDTH = BITONIC_BLOCK_SIZE;
const UINT MATRIX_HEIGHT = NUM_ELEMENTS / BITONIC_BLOCK_SIZE;

/***************************************************************************/
/* This class demonstrates how to use the compute shader we have declared. */
/* Most importantly which RHI functions are needed to call and how to get  */
/* some interesting output.                                                */
/***************************************************************************/
class COMPUTESHADER_API FComputeShader
{
public:
	FComputeShader(float SimulationSpeed, int32 SizeX, int32 SizeY, ERHIFeatureLevel::Type ShaderFeatureLevel);
	~FComputeShader();

	/************************************************************************/
	/* Run this to execute the compute shader once!                         */
	/* @param currentCamPos - The current camera position ! in object space ! of the point cloud proxy mesh.  */
	/************************************************************************/
	void ExecuteComputeShader(FVector4 currentCamPos);

	/************************************************************************/
	/* Only execute this from the render thread!!!                          */
	/************************************************************************/
	void ExecuteComputeShaderInternal();

	/************************************************************************/
	/* Save a screenshot of the target to the project saved folder          */
	/************************************************************************/
	void Save()
	{
		bSave = true;
	}

	FTexture2DRHIRef GetTexture() { return Texture; }

	// Send the reference to the point position data to the compute shader
	void SetPointPosDataReference(TArray<FLinearColor>* data) {
		check(data->Num() <= NUM_ELEMENTS);
		for (int i = 0; i < data->Num(); ++i)
			PointPosData[i] = FVector4((*data)[i]);
	}

private:
	void ParallelBitonicSort(FRHICommandListImmediate& RHICmdList);
	void SaveScreenshot(FRHICommandListImmediate& RHICmdList);

	bool bIsComputeShaderExecuting;
	bool bIsUnloading;
	bool bSave;

	FComputeShaderConstantParameters ConstantParameters;
	FComputeShaderVariableParameters VariableParameters;
	ERHIFeatureLevel::Type FeatureLevel;

	/** Main texture */
	FTexture2DRHIRef Texture;
	FStructuredBufferRHIParamRef Buffer;
	FStructuredBufferRHIParamRef Buffer2;

	TResourceArray<FVector4> PointPosData;

	/** We need a UAV if we want to be able to write to the resource*/
	FUnorderedAccessViewRHIRef TextureUAV;
	FUnorderedAccessViewRHIRef BufferUAV;
	FUnorderedAccessViewRHIRef BufferUAV2;

};
