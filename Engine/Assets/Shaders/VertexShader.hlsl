struct Corretion
{
    float aspecRatio;
    float zoom;
    float sinAngle;
    float cosAngle;
};

struct MatrixBuffer
{
    float4x4 modelMatrix;
    float4x4 arr[3];
};

Corretion corretion : register(b0);

cbuffer CameraBuffer : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 cameraPosition;
}

cbuffer ConstantBuffers : register(b2)
{
    MatrixBuffer arr[256];
};

struct VSOutput
{
    float4 pos : SV_POSITION; // Позиція у clip space
    float4 nor : NORMAL;
    float2 uv : TEXTCOORD;
    uint sideID : SEMANTIC;
};
struct VSInput
{
    float4 pos : POSITION;
    float4 nor : NORMAL;
    float2 uv : TEXCOORD;
    uint index : MYSEM;
};

VSOutput main(VSInput input)
{
    
    VSOutput output;
    if (all(input.nor == float4(0.0, 1.0, 0.0, 0.0))) // Верхня сторона
        output.sideID = 0;
    else if (all(input.nor == float4(0.0, 0.0, 1.0, 0.0))) // Передня сторона
        output.sideID = 1;
    else if (all(input.nor == float4(-1.0, 0.0, 0.0, 0.0))) // Ліва сторона
        output.sideID = 2;
    else if (all(input.nor == float4(0.0, -1.0, 0.0, 0.0))) // Нижня сторона
        output.sideID = 3;
    else if (all(input.nor == float4(1.0, 0.0, 0.0, 0.0))) // Права сторона
        output.sideID = 4;
    else if (all(input.nor == float4(0.0, 0.0, -1.0, 0.0))) // Задня сторона
        output.sideID = 5;
    
    float4 worldPos = mul(arr[input.index].modelMatrix, input.pos);
    output.pos = mul(projectionMatrix, mul(viewMatrix, worldPos));

    output.uv = input.uv;
    output.nor = input.nor;
    return output;
}