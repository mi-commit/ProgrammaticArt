Shader "Graph/Point surface"
{
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface ConfigureSurface Standard FullForwardShadows
        // // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        struct Input{
            float3 worldPos;
         };
         void ConfigureSurface(Input input, inout SurfaceOutputStandard surface){
			surface.Albedo.rb = saturate(input.worldPos.y * (0.51) - 0.49);
            // surface.Albedo.rgb = saturate(input.worldPos.xyz * (0.5) + 0.5);

            // surface.Smoothness = saturate(input.worldPos.y);
         }
        ENDCG
    }
    FallBack "Diffuse"
}
