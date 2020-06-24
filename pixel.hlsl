struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};
Texture2D Texture;
SamplerState sampleState;


float4 main(PixelShaderInput input) : SV_TARGET
{
//return Texture.Sample(sampleState, floor(input.texcoord * 10000 + 0.5) / 10000);
float4 vc = Texture.Sample(sampleState, floor(input.texcoord * 100 + 0.5) / 100);
//return float4(vc.r,0,0,vc.a);
return float4(0,vc.g,0,vc.a);
//return float4(0,0,vc.b,vc.a);
}
