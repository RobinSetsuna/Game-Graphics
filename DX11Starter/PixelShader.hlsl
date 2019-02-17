
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	//float4 color		: COLOR;
};
struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer externalData : register(b0)
{
	DirectionalLight DirLight1;
	DirectionalLight DirLight2;
};

float3 NdotL(float3 normal, float3 dir) {
	float3 DirectionalLightDir = normalize(-dir);
	float dirNdotL = dot(normal, DirectionalLightDir);
	dirNdotL = saturate(dirNdotL);
	return dirNdotL;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	//Directional Light 1
	float3 FinalDirectionalColor1 = NdotL(input.normal, DirLight1.Direction) * DirLight1.DiffuseColor + DirLight1.AmbientColor;

	//Directional Light 2
	float3 FinalDirectionalColor2 = NdotL(input.normal, DirLight2.Direction) * DirLight2.DiffuseColor + DirLight2.AmbientColor;


	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return float4(FinalDirectionalColor1 + FinalDirectionalColor2,1);
}

