#ifndef  DISPLAYFORMEXCEPTION_H
#define  DISPLAYFORMEXCEPTION_H

//Exception : Base
class PrecureModelDisplayerException
{
};

//Exception : Failed to Create Window during Initialization
class PMD_FailCreateWindowException : public PrecureModelDisplayerException
{
};

//Exception : Failed to Create D3D11Device
class PMD_FailCreateD3D11DeviceException : public PrecureModelDisplayerException
{
};

//Exception : Failed to Create SwapChain
class PMD_FailCreateSwapChainException : public PrecureModelDisplayerException
{
};

//Exception : Failed to Create RenderTargetView
class PMD_FailCreateRenderTargetViewException : public PrecureModelDisplayerException
{
};

//Exception : Failed to Create DepthStencilView
class PMD_FailCreateDepthStencilViewException : public PrecureModelDisplayerException
{
};

//Exception : Failed to Query Performance Frequency
class PMD_FailedQueryPerformanceFrequency : public PrecureModelDisplayerException
{
};

//Exception : Failed to Query Performance Counter
class PMD_FailedQueryPerformanceCounter : public PrecureModelDisplayerException
{
};

//Exception : Failed to Initialize Effect
class PMD_FailedInitializeEffect : public PrecureModelDisplayerException
{
};

//Exception : Failed to Create InputLayout
class PMD_FailedCreateInputLayout : public PrecureModelDisplayerException
{
};

//Exception : Failed to Create Buffer
class PMD_FailedCreateBuffer : public PrecureModelDisplayerException
{
};

#endif
