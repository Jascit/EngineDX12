SamplerState textureSampler : register(s0); 

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXTCOORD;
    uint sideID : SEMANTIC;
};

float4 main(PSInput input) : SV_Target
{

    return float4(1.0, 1.0, 1.0, 1.0); // За замовчуванням білий
    
}