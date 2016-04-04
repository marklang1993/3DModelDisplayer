cbuffer cbPerObject
{
	float4x4 matrixWVP;
};

struct vsIn
{
	float3 OPosition: POSITION;
	float4 OColor: COLOR;
};

struct vsOut
{
	float4 HPosition: SV_POSITION;
	float4 HColor: COLOR;
};

vsOut VS(vsIn vin)
{
	vsOut vout;
	
	vout.HPosition = mul(float4(vin.OPosition, 1.0f), matrixWVP);
	vout.HColor = vin.OColor;

	return vout;
}


float4 PS(vsOut pin) : SV_Target
{
	return pin.HColor;
}

technique11 Tech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};
